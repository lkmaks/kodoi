#ifndef ONLINECLASSWIDGET_H
#define ONLINECLASSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

#include "Config.h"
#include "modules/Settings.h"
#include "../OnlineSession.h"

#include "CreateForm.h"
#include "RoomLabel.h"


class OnlineClubWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OnlineClubWidget(Config *config, Settings *settings, OnlineSession *session, QWidget *parent = nullptr);

    void TryCreateNewRoom(RoomId room_id);
    void TryEnterRoom(RoomId room_id);
private:
    Config *config_;
    Settings *settings_;
    OnlineSession *session_;

    QHBoxLayout *hlt;

    CreateForm *create_form_;
    std::vector<RoomLabel*> room_labels_;

    void AddNewRoom(QString room_id);
};

#endif // ONLINECLASSWIDGET_H
