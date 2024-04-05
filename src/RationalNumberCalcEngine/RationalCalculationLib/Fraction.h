#ifndef CALCULATOR_FRACTION_H
#define CALCULATOR_FRACTION_H

#include "BigInt.h"

class Fraction
{
public:
    Fraction() : mNumerator(0), mDenominator(1), mPrecision(12) {};
    explicit Fraction(BigInt  newNumber)
            : mNumerator(std::move(newNumber)), mDenominator(1), mPrecision(12) {
        Simplify();
    }
    Fraction(BigInt numerator, BigInt denominator)
            : mNumerator(std::move(numerator)), mDenominator(std::move(denominator)), mPrecision(12)
    {
        Simplify();
    }
    Fraction(const Fraction& newFraction) {
        mNumerator = newFraction.mNumerator;
        mDenominator = newFraction.mDenominator;
        mPrecision = newFraction.mPrecision;
    }

    [[maybe_unused]] void SetPrecision(uint64_t precision) { mPrecision = precision; }
    void Simplify();
    void BeOppositeNumber();

    [[nodiscard]] bool isZero() const { return (mNumerator.isZero()); }
    [[nodiscard]] bool isNegative() const { return (mNumerator.isNegative()); }
    [[nodiscard]] bool isInteger() const  { return (mDenominator == 1); }

    [[nodiscard]] std::string GetValue() const;
    [[nodiscard]] std::string GetApproximateValue() const;

    Fraction operator+(const Fraction& another) const;
    Fraction operator-(const Fraction& another) const;
    Fraction operator*(const Fraction& another) const;
    Fraction operator/(const Fraction& another) const;
    Fraction operator^(const Fraction& another) const;
    [[nodiscard]] Fraction factorial() const;
    static Fraction CombinationNumber(const Fraction& a, const Fraction& b);
    static Fraction PermutationNumber(const Fraction& a, const Fraction& b);

private:
    // 约定分数的符号以分子的符号为准，且分母一定是正数
    BigInt mNumerator;		// 分子
    BigInt mDenominator;	// 分母
    uint64_t mPrecision;		// 小数点后的精度
};


#endif //CALCULATOR_FRACTION_H
