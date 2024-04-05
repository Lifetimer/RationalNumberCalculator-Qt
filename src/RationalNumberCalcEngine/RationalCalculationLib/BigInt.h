#ifndef CALCULATOR_BIGINT_H
#define CALCULATOR_BIGINT_H

#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief 大数运算类，使用vector储存每一位的数，支持很高的运算精度
 */
class BigInt
{
public:
    BigInt() : mSign(false), mNumber({0}) {};
    BigInt(int number); // NOLINT(*-explicit-constructor) 可以使得语法更舒适
    explicit BigInt(const std::vector<uint8_t> &numSeq);
    [[nodiscard]] std::string GetValue() const;
    [[nodiscard]] std::vector<uint8_t> GetmNumber() const { return mNumber; }
    [[nodiscard]] inline bool isNegative() const { return mSign;};
    BigInt operator*(const BigInt &num) const;
    BigInt operator^(const BigInt& num) const;
    BigInt operator/(const BigInt& num) const;
    BigInt operator%(const BigInt& num) const;
    BigInt operator+(const BigInt& num) const;
    BigInt operator-(const BigInt &num) const;
    bool operator>(const BigInt& num) const;
    bool operator>=(const BigInt& num) const;
    bool operator<(const BigInt& num) const;
    bool operator<=(const BigInt& num) const;
    bool operator==(const BigInt& num) const;
    bool operator!=(const BigInt& num) const;
    void MultiplyByTen();
    void BeAPositiveNumber();
    void BeANegativeNumber();
    static BigInt gcd(BigInt a, BigInt b);


    [[nodiscard]] BigInt factorial() const;
    void InsertNumAsOneDigit(uint8_t oneDigit)
    {
        if (mNumber.size() == 1 && mNumber[0] == 0) {
            mNumber[0] = oneDigit;
            return;
        }
        mNumber.insert(mNumber.begin(), oneDigit);
    }


    [[nodiscard]] BigInt TakePositiveNumber() const
    {
        BigInt returnValue(*this);
        returnValue.mSign = false;
        return returnValue;
    }


    inline void BeTheOppositeNumber()
    {
        if (this->mNumber.size() == 1 && this->mNumber[0] == 0) { return; }
        else {
            this->mSign = !this->mSign;
        }
    }


    [[nodiscard]] bool isZero() const
    {
        if (mNumber.size() == 1 && mNumber[0] == 0) {
            return true;
        }
        return false;
    }


private:
    bool mSign;		// 如果是0表示整数，否则表示负数（0的符号位为0）
    std::vector<uint8_t> mNumber;

private:
    static BigInt CommonSignAdd(const BigInt& num1, const BigInt& num2);
    static BigInt PositiveNumberSub(const BigInt& num1, const BigInt& num2);

    static std::vector<uint64_t> conv(const std::vector<uint8_t> &xn, const std::vector<uint8_t> &hn);
    static std::vector<uint8_t> elementAdd(const std::vector<uint8_t>& xn, const std::vector<uint8_t>& yn);
    static std::vector<int8_t> elementSub(const std::vector<uint8_t>& xn, const std::vector<uint8_t>& yn);

    static std::vector<uint8_t> SimplifyNumSeq(const std::vector<uint8_t> &numSeq);
    static std::vector<uint8_t> SimplifyNumSeq(const std::vector<uint64_t> &numSeq);
    static std::vector<uint8_t> SimplifySubNumSeq(const std::vector<int8_t> &numSeq);

    static char digit2char(uint8_t digit){
        return static_cast<char>('0' + digit);
    }
};

#endif //CALCULATOR_BIGINT_H
