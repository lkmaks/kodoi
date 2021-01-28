#include "OnlineClubWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEventLoop>
#include "MainWindow.h"

OnlineClubWidget::OnlineClubWidget(Config *config, Settings *settings, OnlineSession *session, QWidget *parent) :
    QWidget(parent), config_(config), settings_(settings), session_(session)
{
    create_form_ = new CreateForm(this);
    QVBoxLayout *vlt = new QVBoxLayout(this);
    vlt->addWidget(create_form_);

    hlt = new QHBoxLayout(this);
    vlt->addLayout(hlt);

    session_->RoomsList(); // tell server that we want actual rooms list
    connect(create_form_, &CreateForm::CreateRoom, this, &OnlineClubWidget::TryCreateNewRoom);
    connect(session_, &OnlineSession::ReceivedRoomAdded, this, &OnlineClubWidget::AddNewRoom);
}

void OnlineClubWidget::TryCreateNewRoom(RoomId room_id) {
    session_->CreateRoom(room_id);
}

void OnlineClubWidget::TryEnterRoom(RoomId room_id) {
    session_->EnterRoom(room_id);
    bool res = false;
    QEventLoop loop;
    QObject::connect(session_, &OnlineSession::ReceivedStatus, this, [&loop, &res](QString status){
        res = (status == "ok");
        loop.quit();
    });
    loop.exec();
    if (res) {
        // entering room
        static_cast<MainWindow*>(parent())->EnterRoom(room_id);
    }
}

void OnlineClubWidget::AddNewRoom(QString room_id) {
    auto *label = new RoomLabel(room_id, this);
    hlt->addWidget(label);
    room_labels_.push_back(label);

    connect(label, &RoomLabel::EnterPressed, this, [this, room_id](){TryEnterRoom(room_id);});
}
