#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modules/online/OnlineWidget.h"
#include "modules/explorer/MainWidget.h"
#include "modules/Settings.h"
#include "common/Enums.h"

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
    OnlineWidget *online_widget_;
    Config *config_;
    Settings *settings_;
};

#endif // MAINWINDOW_H
