#include "RationalNumberCalcController.h"
#include "CalcMainWindow.h"

QString RationalNumberCalcController::GetLineText1() {
    if(lineText1.size() >= 25){
        return lineText1.left(5)+QString("......")+lineText1.right(10)+operatorChar;
    }
    else{
        return lineText1+operatorChar;
    }
}

QString RationalNumberCalcController::GetLineText2() {
    if(lineText2.size() >= 25){
        return lineText2.left(5) + QString("......")+lineText2.right(10);
    }
    else{
        return lineText2;
    }
}

QString RationalNumberCalcController::GetDecimalAns() {
    if(decimalAns.size() >= 25){
        return decimalAns.left(25)+"......";
    }
    else{
        return decimalAns;
    }
}

void RationalNumberCalcController::addNewNum(const QString &str) {
    if(operatorChar == "="){
        operatorChar.clear();
    }

    if(str == "|" || str == "."){
        if(     lineText2.contains('.')
                ||  lineText2.contains('|')
                || (lineText2 == "0" && str == "|") )
        {
            return;
        }
        else{
            lineText2.append(str);
        }
    }
        // 否则表示要输入的是一个数字
    else if(lineText2 == "0"){
        lineText2 = str;
    }
    else{
        lineText2.append(str);
    }
    view->UpdateInterface();
    decimalAns = "";
}

void RationalNumberCalcController::changeOperator(const QString &newOperator) {
    if(operatorChar == "="){
        lineText1 = ans;
        operatorChar.clear();
    }

    if(lineText1.isEmpty()){
        lineText1 = lineText2;
        lineText2 = "0";
    }

    if(operatorChar.isEmpty() || lineText2 == "0"){
        operatorChar = newOperator;
    }
    else{
        Calculate();
        lineText1 = ans;
        operatorChar = newOperator;
        lineText2 = "0";
    }
    decimalAns = "";
    view->UpdateInterface();
    if(isUnaryOperator(operatorChar)){
        lineText2.clear();
        Calculate();
    }
}

void RationalNumberCalcController::changeCurrentSign() {
    if(lineText2 == "0" || lineText2.isEmpty()){
        return;
    }
    else{
        if(lineText2[0] == '-'){
            lineText2.erase(lineText2.cbegin());
        }
        else {
            lineText2.insert(0, '-');
        }
    }
    view->UpdateInterface();
    decimalAns = "";
}

void RationalNumberCalcController::clearCurrentInput() {
    if(operatorChar == "="){
        operatorChar.clear();
    }
    lineText2 = "0";
    decimalAns = "";
    view->UpdateInterface();
}

void RationalNumberCalcController::clearAllInput() {
    lineText1 = "";
    operatorChar = "";
    lineText2 = "0";
    decimalAns = "";
    view->UpdateInterface();
}

void RationalNumberCalcController::backspace() {
    if(operatorChar == "="){
        operatorChar.clear();
    }
    if(lineText2.size()>1){
        lineText2.chop(1);
    }
    else{
        lineText2 = "0";
    }
    if(lineText2 == "-"){
        lineText2 = "0";
    }
    decimalAns = "";
    view->UpdateInterface();
}

void RationalNumberCalcController::Calculate() {
    if(operatorChar == "="){
        lineText2 = ans;
        operatorChar.clear();
    }
    try {
        calcEngine->SetCalcNum1(lineText1.toStdString());
        calcEngine->SetOperator(CorrectOperator(operatorChar));
        calcEngine->SetCalcNum2(lineText2.toStdString());
        ans = QString::fromStdString(calcEngine->GetCalculation());
        decimalAns = QString::fromStdString(calcEngine->GetDecimalCalculation());
    }
    catch (std::runtime_error &e){
        ans = QString::fromStdString(e.what());
    }
    lineText1 = lineText1+operatorChar+lineText2+"=";
    operatorChar = "";
    lineText2 = ans;
    view->UpdateInterface();
    if(lineText1.size() >= 25 || lineText2.size() >= 25 || decimalAns.size() >= 25){
        lineText1.chop(1);
        QString temp = lineText1+"\n=" + lineText2;
        if(lineText2 != decimalAns){
            temp+="\n≈";
            temp+=decimalAns;
        }
        CalcMainWindow::DisplayLongResultInNewWindow(temp);
    }
    lineText1 = "";
    operatorChar = "=";
    lineText2 = "0";
}

bool RationalNumberCalcController::isUnaryOperator(const QString &op) {
    if(op == "!"){
        return true;
    }
    else {
        return false;
    }
}

std::string RationalNumberCalcController::CorrectOperator(const QString &op) {
    if(op == "x"){
        return "*";
    }
    else if(op == "÷"){
        return "/";
    }
    else if(op == "nCm"){
        return "C";
    }
    else{
        return op.toStdString();
    }
}

void RationalNumberCalcController::SetPrecision(int newPrecision) {
    calcEngine->SetPrecision(newPrecision);
}

int RationalNumberCalcController::GetPrecision() {
    return calcEngine->GetPrecision();
}
