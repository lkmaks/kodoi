#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(1200, 800);
    setCentralWidget(new MainWidget(this));
}

MainWindow::~MainWindow()
{

}
