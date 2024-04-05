#include "CalcMainWindow.h"
#include "./ui_CalcMainWindow.h"
#include "RationalNumberCalcController.h"
#include <QTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QInputDialog>

CalcMainWindow::CalcMainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::CalcMainWindow)
        , normalCalc(new RationalNumberCalcController(this))
{
    ui->setupUi(this);
    connectSlots();
    ui->label_3->hide();
}

CalcMainWindow::~CalcMainWindow()
{
    delete ui;
    delete normalCalc;
}

void CalcMainWindow::connectSlots() {
    // 连接数字键和点键槽函数
    QPushButton *numButtons[] = {
            ui->btnZero,
            ui->btnOne, ui->btnTwo, ui->btnThree, ui->btnFour,
            ui->btnFive, ui->btnSix, ui->btnSeven, ui->btnEight,
            ui->btnNine, ui->btnPoint
    };
    for(QPushButton *btn: numButtons) {
        connect(btn, &QPushButton::clicked, this, &CalcMainWindow::btnNumPressed);
    }
    connect(ui->btnFractionChar, &QPushButton::clicked, this, &CalcMainWindow::btnFractionCharPressed);

    // 连接操作符槽函数
    QPushButton *operatorButtons[] = {
            ui->btnPlus, ui->btnMinus, ui->btnDiv, ui->btnMultiply, ui->btnPower, ui->btnComb
    };
    for(QPushButton *btn: operatorButtons) {
        connect(btn, &QPushButton::clicked, this, &CalcMainWindow::btnOperatorPressed);
    }
    connect(ui->btnFractorial, &QPushButton::clicked, this, &CalcMainWindow::btnOperatorPressed);


    // 连接功能键槽函数
    connect(ui->btnPNchange, &QPushButton::clicked, this, &CalcMainWindow::btnSignPressed);
    connect(ui->btnCLear, &QPushButton::clicked, this, &CalcMainWindow::btnClearPressed);
    connect(ui->btnClearAll, &QPushButton::clicked, this, &CalcMainWindow::btnClearAllPressed);
    connect(ui->btnBackspace, &QPushButton::clicked, this, &CalcMainWindow::btnBackspacePressed);
    connect(ui->btnEqual, &QPushButton::clicked, this, &CalcMainWindow::btnEqualPressed);

    // 连接动作
    connect(ui->actExit, &QAction::triggered, this, &CalcMainWindow::actExitTriggered);
    connect(ui->actDisplayDecimal, &QAction::triggered, this, &CalcMainWindow::actDisplayDecimalEnabledChanged);
    connect(ui->actSetPreciese,&QAction::triggered, this, &CalcMainWindow::actSetPrecisionWindow);
}

void CalcMainWindow::btnNumPressed() {
    auto *pressedNumButton = qobject_cast<QPushButton*>(sender());
    normalCalc->addNewNum(pressedNumButton->text());
}

void CalcMainWindow::btnSignPressed() {
    normalCalc->changeCurrentSign();
}

void CalcMainWindow::btnClearAllPressed() {
    normalCalc->clearAllInput();
}

void CalcMainWindow::btnClearPressed() {
    normalCalc->clearCurrentInput();
}

void CalcMainWindow::btnBackspacePressed() {
    normalCalc->backspace();
}

void CalcMainWindow::btnFractionCharPressed() {
    normalCalc->addNewNum("|");
}

void CalcMainWindow::btnOperatorPressed() {
    auto *pressedNumButton = qobject_cast<QPushButton*>(sender());
    normalCalc->changeOperator(pressedNumButton->text());
}

void CalcMainWindow::btnEqualPressed() {
    normalCalc->Calculate();
}

void CalcMainWindow::UpdateInterface() {
    ui->label->setText(normalCalc->GetLineText1());
    ui->label_2->setText(normalCalc->GetLineText2());
    ui->label_3->setText(normalCalc->GetDecimalAns());
}

void CalcMainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_0:
            emit ui->btnZero->clicked(); break;
        case Qt::Key_1:
            emit ui->btnOne->clicked();break;
        case Qt::Key_2:
            emit ui->btnTwo->clicked();break;
        case Qt::Key_3:
            emit ui->btnThree->clicked();break;
        case Qt::Key_4:
            emit ui->btnFour->clicked();break;
        case Qt::Key_5:
            emit ui->btnFive->clicked();break;
        case Qt::Key_6:
            emit ui->btnSix->clicked();break;
        case Qt::Key_7:
            emit ui->btnSeven->clicked();break;
        case Qt::Key_8:
            emit ui->btnEight->clicked();break;
        case Qt::Key_9:
            emit ui->btnNine->clicked();break;
        case Qt::Key_Plus:
            emit ui->btnPlus->clicked();break;
        case Qt::Key_Minus:
            emit ui->btnMinus->clicked();break;
        case Qt::Key_Asterisk:
            emit ui->btnMultiply->clicked();break;
        case Qt::Key_Slash:
            emit ui->btnDiv->clicked();break;
        case Qt::Key_Bar:
            emit ui->btnFractionChar->clicked();break;
        case Qt::Key_AsciiCircum:
            emit ui->btnPower->clicked();break;
        case Qt::Key_C:
            emit ui->btnComb->clicked();break;
        case Qt::Key_Return:
        case Qt::Key_Equal:
            emit ui->btnEqual->clicked();break;
        case Qt::Key_Period:
            emit ui->btnPoint->clicked();break;
        case Qt::Key_Exclam:
            emit ui->btnFractorial->clicked();break;
        case Qt::Key_Backspace:
            if(event->modifiers() == Qt::ControlModifier){
                emit ui->btnClearAll->clicked();
            }
            else if(event->modifiers() == Qt::ShiftModifier){
                emit ui->btnCLear->clicked();
            }
            else{
                emit ui->btnBackspace->clicked();
            }
            break;
        default:
            break;
    }
}

void CalcMainWindow::DisplayLongResultInNewWindow(const QString& str) {
    auto labelTitle = new QLabel("运算结果如下：");

    auto *layoutResult = new QVBoxLayout();
    layoutResult->addWidget(labelTitle);

    auto labelContext = new QTextEdit(str);
    labelContext->setReadOnly(true);
    layoutResult->addWidget(labelContext);

    auto *widgetResult = new QWidget();
    widgetResult->setLayout(layoutResult);
    widgetResult->setWindowTitle("计算结果");
    widgetResult->show();
}

void CalcMainWindow::actExitTriggered() {
    QApplication::exit();
}

void CalcMainWindow::actDisplayDecimalEnabledChanged() {
    if(ui->actDisplayDecimal->isChecked()){
        ui->label_3->show();
    }
    else{
        ui->label_3->hide();
    }
}

void CalcMainWindow::actSetPrecisionWindow() {
    int newPrecision = QInputDialog::getInt(this,
                                            "小数精度设置对话框",
                                            "请设置小数点后保留的位数",
                                            normalCalc->GetPrecision(),
                                            1);
    normalCalc->SetPrecision(newPrecision);
}
