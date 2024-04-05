#include "RationalCalcEngine.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

void RationalCalcEngine::SetCalcNum1(const std::string& newNum){
    num1 = StdStringToFraction(newNum);
}

void RationalCalcEngine::SetCalcNum2(const std::string& newNum){
    num2 = StdStringToFraction(newNum);
}

void RationalCalcEngine::SetOperator(const std::string& newOP){
    if (newOP == "+") {
        op = Operator::PLUS;
    }
    else if (newOP == "-") {
        op = Operator::MINUS;
    }
    else if (newOP == "*") {
        op = Operator::MULTIPLY;
    }
    else if (newOP == "/") {
        op = Operator::DIV;
    }
    else if (newOP == "!") {
        op = Operator::FACTORIAL;
    }
    else if (newOP == "C") {
        op = Operator::COMBINATION;
    }
    else if (newOP == "A") {
        op = Operator::ARRANGE;
    }
    else if (newOP == "^") {
        op = Operator::POWER;
    }
    else{
        op = Operator::EMPTY;
    }
}

std::string RationalCalcEngine::GetCalculation()
{
    Calculate();
    return calcResult.GetValue();
}

std::string RationalCalcEngine::GetDecimalCalculation()
{
    Calculate();
    calcResult.SetPrecision(precision);
    return calcResult.GetApproximateValue();
}

void RationalCalcEngine::Calculate(){
    if (op == Operator::PLUS) {
        calcResult = num1 + num2;
    }
    else if (op == Operator::MINUS) {
        calcResult =  num1 - num2;
    }
    else if (op == Operator::MULTIPLY) {
        calcResult = num1 * num2;
    }
    else if (op == Operator::DIV) {
        if (num2.isZero()) {
            throw std::runtime_error("除数不能为0");
        }
        calcResult = num1 / num2;
    }
    else if (op == Operator::FACTORIAL) {
        if (num1.isNegative()) {
            throw std::runtime_error("只能计算正数的阶乘");
        }
        else if (!num1.isInteger()) {
            throw std::runtime_error("只能计算整数的阶乘");
        }
        else {
            calcResult = num1.factorial();
        }
    }
    else if (op == Operator::COMBINATION) {
        if (num1.isNegative() || num2.isNegative()) {
            throw std::runtime_error("只能计算正数的组合数");
        }
        else if (!num1.isInteger() || !num2.isInteger()) {
            throw std::runtime_error("只能计算整数的组合数");
        }
        else {
            calcResult = Fraction::CombinationNumber(num1,num2);
        }
    }
    else if (op == Operator::ARRANGE) {
        if (num1.isNegative() || num2.isNegative()) {
            throw std::runtime_error("只能计算正数的排列数");
        }
        else if (!num1.isInteger() || !num2.isInteger()) {
            throw std::runtime_error("只能计算整数的排列数");
        }
        else {
            calcResult = Fraction::PermutationNumber(num1, num2);
        }
    }
    else if (op == Operator::POWER) {
        if (num1.isZero()) {
            throw std::runtime_error("底数不能为0");
        }
        else if (num2.isNegative()) {
            throw std::runtime_error("指数不能为负数");
        }
        else if (!num1.isInteger() || !num2.isInteger()) {
            throw std::runtime_error("只能对整数进行乘方运算");
        }
        else {
            calcResult = num1 ^ num2;
        }
    }
    else {
        calcResult = num2;
    }
}

Fraction RationalCalcEngine::StdStringToFraction(const std::string& num)
{
    std::vector<uint8_t> numPart1;
    std::vector<uint8_t> numPart2;
    bool sign = false;
    size_t pointCount = 0;
    size_t fractionCount = 0;
    std::string numPart;
    if (num[0] == '-') {
        numPart = std::string(num.begin()+1,num.end());
        sign = true;
    }
    else {
        numPart = num;
    }
    for (const char& element : numPart) {
        if (element == '|') {
            fractionCount++;
        }
        else if (element == '.') {
            pointCount++;
        }
        else if(element >= '0' && element <='9'){}
        else {
            throw std::runtime_error("传入的不是一个合法的数");
        }
    }
    Fraction returnValue;
    if (pointCount == 1 && fractionCount == 0) {
        returnValue = PointStringToFraction(numPart);
    }
    else if (pointCount == 0 && fractionCount == 1) {
        returnValue = FractionStringToFraction(numPart);
    }
    else if (pointCount == 0 && fractionCount == 0) {
        returnValue = NumStringToFraction(numPart);
    }
    else {
        throw std::runtime_error("传入的不是一个合法的数");
    }
    if (sign) {
        returnValue.BeOppositeNumber();
    }
    return returnValue;
}

Fraction RationalCalcEngine::PointStringToFraction(const std::string& numStr)
{
    bool haveMeetPoint = false;
    std::vector<uint8_t> numerator;
    std::vector<uint8_t> denominator({1});
    for (const char& element : numStr) {
        if (element == '.') {
            haveMeetPoint = true;
            continue;
        }
        if (haveMeetPoint) {
            denominator.push_back(0);
        }
        numerator.push_back(element - '0');
    }
    std::reverse(numerator.begin(), numerator.end());
    if (numerator.back() == 0) {
        numerator.pop_back();
    }
    std::reverse(denominator.begin(), denominator.end());
    return {BigInt(numerator),BigInt(denominator)};
}

Fraction RationalCalcEngine::FractionStringToFraction(const std::string& numStr)
{
    bool haveMeetFraction = false;
    std::vector<uint8_t> numerator;
    std::vector<uint8_t> denominator;
    for (const char& element : numStr) {
        if (element == '|') {
            haveMeetFraction = true;
            continue;
        }
        if (haveMeetFraction) {
            denominator.push_back(element - '0');
        }
        else {
            numerator.push_back(element - '0');
        }
    }
    std::reverse(numerator.begin(), numerator.end());
    std::reverse(denominator.begin(), denominator.end());
    return {BigInt(numerator), BigInt(denominator)};
}

Fraction RationalCalcEngine::NumStringToFraction(const std::string& numStr)
{
    std::vector<uint8_t> numerator;
    for (const char& element : numStr) {
        numerator.push_back(element - '0');
    }
    std::reverse(numerator.begin(), numerator.end());
    return {BigInt(numerator), BigInt(1)};
}

[[maybe_unused]] void RationalCalcEngine::SetPrecision(uint64_t newPrecision) {
    precision = newPrecision;
}

int RationalCalcEngine::GetPrecision() const {
    return static_cast<int>(precision);
}
