#include "CalcMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalcMainWindow w;
    w.show();
    return QApplication::exec();
}
