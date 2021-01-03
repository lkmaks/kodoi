#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

#include "modules/online/protocol/Response.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
