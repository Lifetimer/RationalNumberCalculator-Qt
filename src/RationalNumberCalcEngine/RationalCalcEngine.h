#ifndef CALCULATOR_RATIONALCALCENGINE_H
#define CALCULATOR_RATIONALCALCENGINE_H

#include "./RationalCalculationLib/BigInt.h"
#include "./RationalCalculationLib/Fraction.h"
#include <string>

enum class Operator : uint8_t
{
    EMPTY = 0,
    PLUS = 1,
    MINUS = 2,
    MULTIPLY = 3,
    DIV = 4,
    FACTORIAL = 5,
    COMBINATION = 6,
    ARRANGE = 7,
    POWER = 8
};

class RationalCalcEngine
{
public:
    RationalCalcEngine() : num1(), num2(), op(Operator::EMPTY), calcResult(), precision(16) {};
    void SetCalcNum1(const std::string&);
    void SetCalcNum2(const std::string&);
    void SetOperator(const std::string&);

    [[maybe_unused]] void SetPrecision(uint64_t newPrecision);
    int GetPrecision() const;
    std::string GetCalculation();
    std::string GetDecimalCalculation();

private:
    static Fraction StdStringToFraction(const std::string&);
    static Fraction PointStringToFraction(const std::string&);
    static Fraction FractionStringToFraction(const std::string&);
    static Fraction NumStringToFraction(const std::string&);
    void Calculate();

private:
    Fraction num1;
    Fraction num2;
    Operator op;
    Fraction calcResult;
    uint64_t precision;
};

#endif //CALCULATOR_RATIONALCALCENGINE_H
