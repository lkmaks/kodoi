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

    void OnlineCreate();
    void OnlineEnter();

signals:
    void SettingsUpdated(SettingsField field);

    void OnlineRoomCreate(RoomId room_id);
    void OnlineRoomEnter(RoomId room_id);

private:
    OnlineWidget *online_widget_;
    Settings *settings_;
};

#endif // MAINWINDOW_H
