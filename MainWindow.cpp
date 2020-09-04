#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet("background-color:#dddddd;");
    setMinimumSize(1200, 800);
    setCentralWidget(new MainWidget(this));
}

MainWindow::~MainWindow()
{

}
