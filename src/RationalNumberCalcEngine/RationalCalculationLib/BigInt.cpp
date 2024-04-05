#include "BigInt.h"
#include <algorithm>
#include <stdexcept>

BigInt::BigInt(int number){
    // 先赋值符号位
    mSign = number < 0;
    // 然后舍弃符号位，提取数字部分
    number = abs(number);
    // 如果这个数就是0
    if (number == 0) {
        mNumber.push_back(0);
    }
    // 从个位开始依次取数字进入向量
    while (number > 0) {
        mNumber.push_back(number % 10);
        number /= 10;
    }
}

BigInt::BigInt(const std::vector<uint8_t>& numSeq){
    if (numSeq.empty()) {
        mSign = false;
        mNumber.push_back(0);
    }
    else {
        mSign = false;
        mNumber = numSeq;
    }
}

std::string BigInt::GetValue() const
{
    std::string returnValue;
    for (uint8_t digit : mNumber) {
        returnValue.push_back(digit2char(digit));
    }
    if (mSign) {
        returnValue += '-';
    }
    std::reverse(returnValue.begin(), returnValue.end());
    return returnValue;
}

BigInt BigInt::operator*(const BigInt& num) const
{
    if (num == 1) {
        return *this;
    }
    else if (num == 0 || *this == 0) {
        return 0;
    }
    // 乘法就是先卷积再进位
    BigInt returnValue(SimplifyNumSeq(conv(this->mNumber, num.mNumber)));
    returnValue.mSign = this->mSign ^ num.mSign;	// 相异即为负
    return returnValue;
}

BigInt BigInt::operator^(const BigInt& num) const
{
    if (num.isNegative()) {
        throw std::runtime_error("指数不能为负数");
    }
    else if (num.isZero()) {
        return 1;
    }
    else if (this->isZero()) {
        throw std::runtime_error("底数不能为0");
    }
    else {
        BigInt returnValue(1);
        for (BigInt i(1); i <= num; i = i + 1) {
            returnValue = returnValue * (*this);
        }
        return returnValue;
    }
}

BigInt BigInt::operator/(const BigInt& num) const
{
    if (num == 1) {
        return *this;
    }
    if (this->mNumber.size() >= num.mNumber.size())
    {
        BigInt temp;	// 这个temp在num<0时会变成num的相反数
        const BigInt* divisor;	// 这里不能初始化，需要根据后文判断是绑定在num还是其相反数上
        if (num < 0) {
            temp = num.TakePositiveNumber();
            divisor = &temp;
        }
        else if (num == 0) {
            throw std::runtime_error("除数不能为零！");
        }
        else {
            divisor = &num;
        }
        std::vector<uint8_t> result;
        auto initialLength = static_cast<long long>(divisor->mNumber.size());
        long long scanOffset = initialLength + 1;
        BigInt dividend(
                std::vector<uint8_t>(this->mNumber.end() - initialLength, this->mNumber.end())
        );	// 待除向量
        while (true) {
            uint8_t count = 0;	// 商中那一位的值（从高位开始计算）
            while (dividend >= *divisor) {
                dividend = dividend - *divisor;
                count++;
            }
            result.push_back(count);
            if (scanOffset > this->mNumber.size()) {
                // 除到底了，如果是整除就可以结束了
                break;
            }
            dividend.InsertNumAsOneDigit(*(this->mNumber.end() - scanOffset));
            scanOffset++;
        }
        std::reverse(result.begin(), result.end());
        while ((!result.empty()) && result.back() == 0) {
            result.pop_back();
        }
        if (result.empty()) {
            result.push_back(0);
        }
        BigInt returnValue(result);
        if (this->mSign ^ num.mSign) {
            returnValue.mSign = true;
        }
        if (returnValue.mNumber.size() == 1 && returnValue.mNumber[0] == 0) {
            returnValue.mSign = false;
        }
        return returnValue;
    }
    else
    {
        return 0;
    }
}

/**
 * 取模运算结果的正负是由左操作数的正负决定的。
 * 如果%左操作数是正数，那么取模运算的结果是非负数；
 * 如果%左操作数是负数，那么取模运算的结果是负数或0。.
 *
 */
BigInt BigInt::operator%(const BigInt& num) const
{
    if (num == 1) {
        return 0;
    }
    if (this->mNumber.size() < num.mNumber.size()) {
        return *this;
    }
    BigInt temp;	// 这个temp在num<0时会变成num的相反数
    const BigInt* divisor;	// 这里不能初始化，需要根据后文判断是绑定在num还是其相反数上
    if (num < 0) {
        temp = num.TakePositiveNumber();
        divisor = &temp;
    }
    else if (num == 0) {
        throw std::runtime_error("模数不能为零！");
    }
    else {
        divisor = &num;
    }
    auto initialLength = static_cast<long long>(divisor->mNumber.size()) - 1;
    BigInt dividend(
            std::vector<uint8_t>(this->mNumber.end() - initialLength, this->mNumber.end())
    );
    for (auto index = static_cast<long long>(this->mNumber.size()) - initialLength - 1;
         index >= 0; index--) {
        dividend.InsertNumAsOneDigit(*(this->mNumber.begin() + index));
        while (dividend >= *divisor) {
            dividend = dividend - *divisor;
        }
    }
    if (this->mSign) {
        if (dividend.mNumber.size() == 1 && dividend.mNumber[0] == 0) ;
        else {
            dividend.BeTheOppositeNumber();
        }
    }
    return dividend;
}

BigInt BigInt::operator+(const BigInt& num) const
{
    if (num.isZero()) {
        return *this;
    }
    if(this->mSign){
        // 如果自己是负数
        if(num.mSign){
            // 两者都是负数
            return CommonSignAdd(*this, num);
        }
        else{
            // num是正数
            return PositiveNumberSub(num,this->TakePositiveNumber());
        }
    }
    else {
        // 自己是正数
        if(num.mSign){
            // num是负数
            return PositiveNumberSub(*this, num.TakePositiveNumber());

        }
        else{
            // 两者都是正数
            return CommonSignAdd(*this, num);
        }
    }
}

BigInt BigInt::CommonSignAdd(const BigInt& num1, const BigInt& num2){
    // 将两个相同的符号的数相加, 不包含运行时校验
    std::vector<uint8_t> resultSeq = SimplifyNumSeq(elementAdd(num1.mNumber, num2.mNumber));
    BigInt result(resultSeq);
    result.mSign = num1.mSign;
    return result;
}

BigInt BigInt::PositiveNumberSub(const BigInt &num1, const BigInt &num2) {
    // 要求num1和num2都是正数！不含运行时校验
    const BigInt* bigger;
    const BigInt* smaller;
    bool num1IsBigger;
    if (num1 == num2) {
        return 0;
    }
    else if (num1 > num2) {
        bigger = &num1;
        smaller = &num2;
        num1IsBigger = true;
    }
    else {
        bigger = &num2;
        smaller = &num1;
        num1IsBigger = false;
    }
    std::vector<uint8_t> subResult = SimplifySubNumSeq(elementSub(bigger->mNumber, smaller->mNumber));
    BigInt result(subResult);
    result.mSign = !num1IsBigger;
    return result;
}

BigInt BigInt::operator-(const BigInt &num) const
{
    if (num.isZero()) {
        return *this;
    }
    if(this->mSign){
        // 自己是负数
        if(num.mSign){
            // num是负数
            return PositiveNumberSub(num.TakePositiveNumber(),this->TakePositiveNumber());
        }
        else{
            // num是正数
            return CommonSignAdd(*this,num.TakePositiveNumber());
        }
    }
    else{
        // 自己是正数
        if(num.mSign){
            // num是负数
            return CommonSignAdd(*this,num.TakePositiveNumber());
        }
        else{
            // num是正数
            return PositiveNumberSub(*this,num);
        }
    }
}

bool BigInt::operator>(const BigInt& num) const
{
    if (this->isNegative() && (!num.isNegative())) {
        return false;
    }
    else if ((!this->isNegative()) && num.isNegative()) {
        return true;
    }
    else if (!(this->isNegative())) {
        if (this->mNumber.size() > num.mNumber.size()) {
            return true;
        }
        else if (this->mNumber.size() < num.mNumber.size()) {
            return false;
        }
        else {
            for (auto i = static_cast<long long>(mNumber.size()) - 1; i >= 0; i--) {
                if (this->mNumber[i] > num.mNumber[i]) {
                    return true;
                }
                else if (this->mNumber[i] < num.mNumber[i]) {
                    return false;
                }
            }
            return false;
        }
    }
    else {
        if (this->mNumber.size() > num.mNumber.size()) {
            return false;
        } else if (this->mNumber.size() < num.mNumber.size()) {
            return true;
        } else {
            for (auto i = static_cast<long long>(mNumber.size()) - 1; i >= 0; i--) {
                if (this->mNumber[i] > num.mNumber[i]) {
                    return false;
                } else if (this->mNumber[i] < num.mNumber[i]) {
                    return true;
                }
            }
            return false;
        }
    }
}

bool BigInt::operator>=(const BigInt& num) const
{
    if (this->isNegative() && (!num.isNegative())) {
        return false;
    }
    else if ((!this->isNegative()) && num.isNegative()) {
        return true;
    }
    else if (!this->isNegative()) {
        if (this->mNumber.size() > num.mNumber.size()) {
            return true;
        }
        else if (this->mNumber.size() < num.mNumber.size()) {
            return false;
        }
        else {
            for (auto i = static_cast<long long>(mNumber.size()) - 1; i >= 0; i--) {
                if (this->mNumber[i] > num.mNumber[i]) {
                    return true;
                }
                else if (this->mNumber[i] < num.mNumber[i]) {
                    return false;
                }
            }
            return true;
        }
    }
    else {
        if (this->mNumber.size() > num.mNumber.size()) {
            return false;
        } else if (this->mNumber.size() < num.mNumber.size()) {
            return true;
        } else {
            for (auto i = static_cast<long long>(mNumber.size()) - 1; i >= 0; i--) {
                if (this->mNumber[i] > num.mNumber[i]) {
                    return false;
                } else if (this->mNumber[i] < num.mNumber[i]) {
                    return true;
                }
            }
            return true;
        }
    }
}

bool BigInt::operator<(const BigInt& num) const
{
    return !(operator>=(num));
}

bool BigInt::operator<=(const BigInt& num) const
{
    return !(operator>(num));
}

bool BigInt::operator==(const BigInt& num) const
{
    if (this->mSign ^ num.mSign) {
        return false;
    }
    else {
        if (this->mNumber.size() != num.mNumber.size()) {
            return false;
        }
        else {
            for (int i = 0; i < mNumber.size(); i++) {
                if (this->mNumber[i] != num.mNumber[i]) {
                    return false;
                }
            }
            return true;
        }
    }
}

bool BigInt::operator!=(const BigInt& num) const
{
    return !(*this == num);
}

/**
 * @brief 对一个高精度整数乘10的专门优化函数
 *
 */
void BigInt::MultiplyByTen(){
    if (mNumber.size() == 1 && mNumber[0] == 0) {
        return;
    }
    mNumber.insert(mNumber.begin(), 0);
}

void BigInt::BeAPositiveNumber(){
    mSign = false;
}

void BigInt::BeANegativeNumber(){
    if (this->isZero()) {
        mSign = false;
    }
    else {
        mSign = true;
    }
}

BigInt BigInt::factorial() const
{
    if (*this < 0) {
        throw std::runtime_error("必须要正数才可以阶乘！");
    }
    BigInt remainder(*this);
    BigInt returnValue(1);
    while (remainder > 1) {
        returnValue = returnValue * remainder;
        remainder = remainder - 1;
    }
    return returnValue;
}

BigInt BigInt::gcd(BigInt a, BigInt b)
{
    if (a.isNegative()) {
        a.BeTheOppositeNumber();
    }
    if (b.isNegative()) {
        b.BeTheOppositeNumber();
    }
    if (a.isZero()) {
        return b;
    }
    else if (b.isZero()) {
        return a;
    }
    if (a == 1 || b == 1) {
        return 1;
    }
    BigInt tempInt;
    BigInt* dividend;
    BigInt* divisor;
    BigInt* remainder = &tempInt;
    if (a >= b) {
        dividend = &a;
        divisor = &b;
    }
    else {
        divisor = &a;
        dividend = &b;
    }
    BigInt* temp;
    for (; !divisor->isZero();
           temp = dividend, dividend = divisor, divisor = remainder, remainder = temp) {
        *remainder = (*dividend) % (*divisor);
    }
    return *dividend;
}

std::vector<uint8_t> BigInt::SimplifyNumSeq(const std::vector<uint8_t> &numSeq)
{
    std::vector<uint8_t> num(numSeq);
    for (size_t index = 0; index < num.size(); index++) {
        uint8_t thisDigit = num[index];
        num[index] = thisDigit % 10;
        if (thisDigit / 10 != 0) {
            if (index + 1 >= num.size()) {
                num.push_back(thisDigit / 10);
            }
            else {
                num[index + 1] += thisDigit / 10;
            }
        }
    }
    return num;
}

std::vector<uint8_t> BigInt::SimplifyNumSeq(const std::vector<uint64_t> &numSeq)
{
    std::vector<uint8_t> resultSeq;
    uint64_t carryTemp = 0;		// 进位缓存
    for (size_t index = 0; index < numSeq.size(); index++) {
        uint64_t thisDigit = numSeq[index] + carryTemp;
        carryTemp = 0;
        resultSeq.push_back(static_cast<uint8_t>(thisDigit % 10));
        if (thisDigit / 10 != 0) {
            if (index + 1 >= numSeq.size()) {
                resultSeq.push_back(static_cast<uint8_t>(thisDigit / 10));
            }
            else {
                carryTemp += thisDigit / 10;
            }
        }
    }
    return resultSeq;
}

/**
 * @brief 专门用于计算减法的结果序列处理（借位操作
 *		!!!!!传入的序列必须是大数减小数之后的序列！！！！！！
 *
 * @param numSeq 逐元素相减之后的序列元素
 *
 * @return 相减之后的结果序列（低位在前高位在后）
 */
std::vector<uint8_t> BigInt::SimplifySubNumSeq(const std::vector<int8_t> &numSeq) {
    // 先做有效性判断保证一定是大的元素减小的元素
    // 判断原理：从高位至低位第一个非零数字的大小大小大于0即为合法
    for (int64_t i = (static_cast<int64_t>(numSeq.size()) - 1); i >= 0; i--) {
        if (numSeq[i] < 0) {
            throw std::runtime_error("SimplifySubNumSeq()接受的序列必须为大数减小数的序列");
        }
        else if (numSeq[i] > 0) {
            // 表示这个序列确实是大数减小数得到的序列
            break;
        }
        // 如果等于0就不管，向前遍历直至发现第一个非零元素
    }
    std::vector<uint8_t> result;
    // 判断完成，正式开始借位
    int8_t borrowTemp = 0;	// 借了一位就是1
    for (int8_t element : numSeq) {
        auto thisDigit = static_cast<int8_t>(element - borrowTemp);
        borrowTemp = 0;
        if (thisDigit < 0) {
            thisDigit += 10;
            borrowTemp = 1;
        }
        result.push_back(static_cast<uint8_t>(thisDigit));
    }
    while ((!result.empty()) && result.back() == 0) {
        result.pop_back();
    }
    if (result.empty()) {
        result.push_back(0);
    }
    return result;
}

/**
 * @brief 计算两个序列的离散卷积
 *
 * @param xn 序列x[n]
 * @param hn 序列h[n]
 * @return y[n] = x[n]*h[n]
 */
std::vector<uint64_t> BigInt::conv(const std::vector<uint8_t> &xn, const std::vector<uint8_t> &hn)
{
    std::vector<uint64_t> returnValue;
    size_t xnLength = xn.size();
    size_t hnLength = hn.size();
    size_t resultLength = xnLength + hnLength - 1;
    // n是结果序列中每一项的下标，（从0开始到两原序列和的减一）
    for (size_t n = 0; n < resultLength; n++) {
        // 先确定k的范围(由数学知识)：
        // k >= max(0,n - hnLength + 1)
        // k <= min(xnLength - 1, n)
        size_t kMin = (n <= hnLength-1) ? 0 : n - hnLength + 1;
        size_t kMax = std::min(xnLength - 1, n);
        uint64_t digitResult = 0;
        // 代入卷积和公式
        for (size_t k = kMin; k <= kMax; k++) {
            digitResult += static_cast<uint64_t>(xn[k]) * static_cast<uint64_t>(hn[n - k]);
        }
        returnValue.push_back(digitResult);
    }
    return returnValue;
}

/**
 * @brief 计算两个序列的各位置元素对应和
 *
 * @param xn 序列x[n]
 * @param yn 序列y[n]
 * @return z[n] = x[n]+y[n]
 */
std::vector<uint8_t> BigInt::elementAdd(const std::vector<uint8_t>& xn, const std::vector<uint8_t>& yn)
{
    std::vector<uint8_t> returnValue;
    // 确定 longer shorter 序列
    const std::vector<uint8_t>* longer;
    const std::vector<uint8_t>* shorter;
    if (xn.size() >= yn.size()) {
        longer = &xn;
        shorter = &yn;
    }
    else {
        longer = &yn;
        shorter = &xn;
    }
    // 确定需要相加的部分
    for (size_t i = 0; i < shorter->size(); i++) {
        returnValue.push_back(xn[i] + yn[i]);
    }
    // 不需要相加的部分直接复制
    for (size_t i = shorter->size(); i < longer->size(); i++) {
        returnValue.push_back((*longer)[i]);
    }
    return returnValue;
}

/**
 * @brief 计算两个序列的各位置元素对应差
 *
 * @param xn 序列x[n]
 * @param yn 序列y[n]
 * @return z[n] = x[n]-y[n]
 */
std::vector<int8_t> BigInt::elementSub(const std::vector<uint8_t>& xn, const std::vector<uint8_t>& yn)
{
    std::vector<int8_t> result;
    size_t maxLength = std::max(xn.size(), yn.size());
    for (size_t i = 0; i < maxLength; i++) {
        const uint8_t& x = (i < xn.size()) ? xn[i] : 0;
        const uint8_t& y = (i < yn.size()) ? yn[i] : 0;
        result.push_back((int8_t)(int32_t(x) - int32_t(y)));
    }
    return result;
}
