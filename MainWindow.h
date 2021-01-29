#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Config.h"
#include "modules/Settings.h"

#include "modules/online/Room/OnlineWidget.h"
#include "modules/online/Club/OnlineClubWidget.h"
#include "modules/online/Club/LoginWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void EngineSetup();

    void LoginSuccessful(QString name, QString password, bool guest = false);
    void EnterRoom(RoomId);
    void LeaveRoom();

signals:
    void SettingsUpdated(SettingsField field);

private:
    Config *config_;
    Settings *settings_;

    OnlineSession *online_session_;

    QWidget *cur_widget_;
};

#endif // MAINWINDOW_H
