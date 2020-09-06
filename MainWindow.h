#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MainWidget.h"
#include "Settings.h"
#include "Enums.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void EngineSetup();

signals:
    void SettingsUpdated(SettingsField field);

private:
    MainWidget *main_widget_;
    Settings *settings_;
    QMenu *menu_;
};

#endif // MAINWINDOW_H
