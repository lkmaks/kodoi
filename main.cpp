#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    QFont t;
//    qDebug() << t.pointSizeF() << endl;
//    t.setPointSizeF(3.5);
//    qDebug() << t.pointSizeF() << endl;
//    return 0;

    return a.exec();
}
