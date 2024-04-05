#ifndef CALCMAINWINDOW_H
#define CALCMAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

class RationalNumberCalcController;

QT_BEGIN_NAMESPACE
namespace Ui {
class CalcMainWindow;
}
QT_END_NAMESPACE

class CalcMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalcMainWindow(QWidget *parent = nullptr);
    ~CalcMainWindow() override;
    void UpdateInterface();
    static void DisplayLongResultInNewWindow(const QString& str);

private slots:
    void btnNumPressed();
    void btnSignPressed();
    void btnClearAllPressed();
    void btnClearPressed();
    void btnBackspacePressed();
    void btnFractionCharPressed();
    void btnOperatorPressed();
    void btnEqualPressed();

    static void actExitTriggered();
    void actDisplayDecimalEnabledChanged();
    void actSetPrecisionWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void connectSlots();

private:
    Ui::CalcMainWindow *ui;
    RationalNumberCalcController *normalCalc;
};
#endif // CALCMAINWINDOW_H
