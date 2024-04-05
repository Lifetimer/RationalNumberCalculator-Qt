#include "Fraction.h"
#include <stdexcept>
#include <vector>
#include <algorithm>

void Fraction::Simplify()
{
    if (mNumerator.isZero()) {
        mDenominator = 1;
        return;
    }
    if (mNumerator.isNegative() ^ mDenominator.isNegative()) {
        mNumerator.BeANegativeNumber();
        mDenominator.BeAPositiveNumber();
    }
    else {
        mNumerator.BeAPositiveNumber();
        mDenominator.BeAPositiveNumber();
    }
    if (mNumerator.isZero()) {
        mDenominator = 1;
    }
    else {
        BigInt divisor = BigInt::gcd(mNumerator, mDenominator);
        if (divisor.isZero()) {
            throw std::runtime_error("分母不能为0");
        }
        mNumerator = mNumerator / divisor;
        mDenominator = mDenominator / divisor;
    }
}

void Fraction::BeOppositeNumber(){
    mNumerator.BeTheOppositeNumber();
}

std::string Fraction::GetValue() const
{
    std::string returnValue;
    returnValue += mNumerator.GetValue();
    if (mDenominator == 1) {
        return returnValue;
    }
    else {
        returnValue += '|';
        returnValue += mDenominator.GetValue();
        return returnValue;
    }
}

std::string Fraction::GetApproximateValue() const
{
    if (mDenominator == 1) {
        return mNumerator.GetValue();
    }
    if (mNumerator.GetmNumber().size() >= mDenominator.GetmNumber().size()) {
        std::string returnValue;
        const std::vector<uint8_t> mNumeratorNumber = mNumerator.GetmNumber();
        auto initialLength = static_cast<long long>(mDenominator.GetmNumber().size());
        long long scanOffset = initialLength + 1;
        BigInt dividend(
                std::vector<uint8_t>(mNumeratorNumber.end() - initialLength, mNumeratorNumber.end())
        );	// 待除向量
        bool endFlag = false;
        size_t precisionCounts = 0;
        while (true) {
            uint8_t count = 0;	// 商中那一位的值（从高位开始计算）
            while (dividend >= mDenominator) {
                dividend = dividend - mDenominator;
                count++;
            }
            returnValue.push_back(static_cast<char>('0' + count));
            if (scanOffset == mNumeratorNumber.size() + 1) {
                // 除到底了
                returnValue.push_back('.');
                endFlag = true;
            }
            if (endFlag) {
                dividend.MultiplyByTen();
                precisionCounts++;
            }
            else {
                dividend.InsertNumAsOneDigit(*(mNumeratorNumber.end() - scanOffset));
            }
            if (precisionCounts == mPrecision + 1) {
                break;
            }
            scanOffset++;
        }
        while ((!returnValue.empty()) && returnValue.back() == '0') {
            returnValue.pop_back();
        }
        if (returnValue.empty()) {
            returnValue.push_back(0);
        }
        std::reverse(returnValue.begin(), returnValue.end());
        while ((!returnValue.empty()) && returnValue.back() == '0') {
            returnValue.pop_back();
        }
        if (returnValue.empty()) {
            returnValue.push_back('0');
        }
        if (returnValue.back() == '.') {
            returnValue.push_back('0');
        }
        if (this->isNegative()) {
            returnValue += '-';
        }
        std::reverse(returnValue.begin(), returnValue.end());
        return returnValue;
    }
    else {
        std::string returnValue("0.");
        const std::vector<uint8_t> mNumeratorNumber = mNumerator.GetmNumber();
        const std::vector<uint8_t> mDenominatorNumber = mDenominator.GetmNumber();
        size_t initialLength = mDenominatorNumber.size();
        std::vector<uint8_t> dividendVector(mNumeratorNumber.begin(), mNumeratorNumber.end());
        size_t precisionCounts = 0;
        dividendVector.insert(dividendVector.begin(), 0);
        while (dividendVector.size() < initialLength) {
            dividendVector.insert(dividendVector.begin(), 0);
            returnValue.push_back('0');
            precisionCounts++;
            if (precisionCounts >= mPrecision) {
                return "0";
            }
        }
        BigInt dividend(dividendVector);	// 待除向量
        while (true) {
            uint8_t count = 0;	// 商中那一位的值（从高位开始计算）
            while (dividend >= mDenominator) {
                dividend = dividend - mDenominator;
                count++;
            }
            returnValue.push_back(static_cast<char>('0' + count));
            dividend.MultiplyByTen();
            precisionCounts++;
            if (precisionCounts == mPrecision) {
                break;
            }
        }
        while ((!returnValue.empty()) && returnValue.back() == '0') {
            returnValue.pop_back();
        }
        if (returnValue.empty()) {
            returnValue.push_back(0);
        }
        if (this->isNegative()) {
            returnValue.insert(returnValue.begin(),'-');
        }
        return returnValue;
    }
}

Fraction Fraction::operator+(const Fraction& another) const
{
    BigInt newDenominator((this->mDenominator) * (another.mDenominator));
    BigInt newNumerator(
            (this->mNumerator * another.mDenominator) + (this->mDenominator * another.mNumerator)
    );
    Fraction returnValue(newNumerator, newDenominator);
    returnValue.Simplify();
    return returnValue;
}

Fraction Fraction::operator-(const Fraction& another) const
{
    BigInt newDenominator((this->mDenominator) * (another.mDenominator));
    BigInt newNumerator(
            (this->mNumerator * another.mDenominator) - (this->mDenominator * another.mNumerator)
    );
    Fraction returnValue(newNumerator, newDenominator);
    returnValue.Simplify();
    return returnValue;
}

Fraction Fraction::operator*(const Fraction& another) const
{
    BigInt newDenominator((this->mDenominator) * (another.mDenominator));
    BigInt newNumerator((this->mNumerator * another.mNumerator));
    Fraction returnValue(newNumerator, newDenominator);
    returnValue.Simplify();
    return returnValue;
}

Fraction Fraction::operator/(const Fraction& another) const
{
    if (another.isZero()) {
        throw std::runtime_error("除数不能为0");
    }
    BigInt newDenominator((this->mDenominator) * (another.mNumerator));
    BigInt newNumerator((this->mNumerator * another.mDenominator));
    Fraction returnValue(newNumerator, newDenominator);
    returnValue.Simplify();
    return returnValue;
}

Fraction Fraction::operator^(const Fraction& another) const
{
    if (mDenominator != 1) {
        throw std::runtime_error("只允许对整数做阶乘运算");
    }
    else if (this->mNumerator.isZero()) {
        throw std::runtime_error("底数不能为0");
    }
    else if (another.mNumerator.isNegative()) {
        throw std::runtime_error("指数不能小于0");
    }
    else {
        return Fraction(this->mNumerator ^ another.mNumerator);
    }
}

Fraction Fraction::factorial() const
{
    if (!(mDenominator == 1)) {
        throw std::runtime_error("只允许对整数做阶乘运算");
    }
    if (mNumerator.isNegative()) {
        throw std::runtime_error("只允许对正数和0做阶乘运算");
    }
    return Fraction(mNumerator.factorial());
}

Fraction Fraction::CombinationNumber(const Fraction& n, const Fraction& m)
{
    if ((!(n.mDenominator == 1)) || (!(m.mDenominator == 1))) {
        throw std::runtime_error("组合数的两个数必须是整数！");
    }
    if (n.mNumerator.isNegative() || m.mNumerator.isNegative()) {
        throw std::runtime_error("组合数的两个数必须是正数！");
    }
    if (n.mNumerator < m.mNumerator) {
        throw std::runtime_error("n必须大于m");
    }
    BigInt newDenominator(
            (m.mNumerator.factorial())*((n.mNumerator - m.mNumerator).factorial())
    );
    BigInt newNumerator(
            n.mNumerator.factorial()
    );
    Fraction returnValue(newNumerator, newDenominator);
    returnValue.Simplify();
    return returnValue;
}

Fraction Fraction::PermutationNumber(const Fraction& n, const Fraction& m)
{
    if ((!(n.mDenominator == 1)) || (!(m.mDenominator == 1))) {
        throw std::runtime_error("排列数的两个数必须是整数！");
    }
    if (n.mNumerator.isNegative() || m.mNumerator.isNegative()) {
        throw std::runtime_error("排列数的两个数必须是正数！");
    }
    if (n.mNumerator < m.mNumerator) {
        throw std::runtime_error("n必须大于m");
    }
    BigInt newDenominator(
            (n.mNumerator - m.mNumerator).factorial()
    );
    BigInt newNumerator(
            n.mNumerator.factorial()
    );
    Fraction returnValue(newNumerator, newDenominator);
    returnValue.Simplify();
    return returnValue;
}
