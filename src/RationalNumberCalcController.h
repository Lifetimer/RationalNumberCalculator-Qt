#ifndef CALCULATOR_RATIONALNUMBERCALCCONTROLLER_H
#define CALCULATOR_RATIONALNUMBERCALCCONTROLLER_H

#include <QString>
#include "RationalNumberCalcEngine/RationalCalcEngine.h"

class CalcMainWindow;

class RationalNumberCalcController {
public:
    explicit RationalNumberCalcController(CalcMainWindow *parent)
    : view(parent) ,lineText1(), lineText2("0"), decimalAns(),
            operatorChar(), ans(), calcEngine(new RationalCalcEngine()){
    };

    ~RationalNumberCalcController(){
        delete calcEngine;
    }

    QString GetLineText1();
    QString GetLineText2();
    QString GetDecimalAns();
    void addNewNum(const QString&);
    void changeOperator(const QString &newOperator);
    void changeCurrentSign();
    void clearCurrentInput();
    void clearAllInput();
    void backspace();
    void Calculate();
    void SetPrecision(int newPrecision);
    int GetPrecision();

private:
    static bool isUnaryOperator(const QString& op);         // 判断运算符op是否是一元运算符
    static std::string CorrectOperator(const QString& op);

private:
    CalcMainWindow *view;

    QString lineText1;
    QString lineText2;
    QString decimalAns;
    QString operatorChar;

    QString ans;    // 保存上次的计算结果

    RationalCalcEngine *calcEngine;
};


#endif //CALCULATOR_RATIONALNUMBERCALCCONTROLLER_H
