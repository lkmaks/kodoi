#include "OnlineClient.h"
#include "protocol/Response.h"
#include <QThread>


OnlineClient::OnlineClient(QObject *parent) : QObject(parent)
{
    data_ = new QByteArray();
    sock_ = new QTcpSocket();
    connect(sock_, &QTcpSocket::readyRead, this, &OnlineClient::SocketReadyRead);
    sock_->connectToHost("54.146.159.82", 12345);
    //sock_->connectToHost("127.0.0.1", 12345);
    sock_->waitForConnected();
    last_msg_time_ = QDateTime::currentDateTime();
}

void OnlineClient::SetRoomId(RoomId room_id) {
    room_id_ = room_id;
}

void OnlineClient::Create(RoomId room_id) {
    Message msg;
    msg.type = MessageType::CREATE;
    msg.room_id = room_id;
    SendMessage(msg);
}

void OnlineClient::Enter(RoomId room_id) {
    Message msg;
    msg.type = MessageType::ENTER;
    msg.room_id = room_id;
    SendMessage(msg);

    /// dont want to include this yet:

    // this is kind of premature, but if you use client correcly
    // (meaning do not use non-last enter), its fine
    //room_id_ = room_id;
}

void OnlineClient::MakeBoardAction(BoardAction action) {
    Message msg;
    msg.type = MessageType::ACTION;
    msg.room_id = room_id_;
    msg.action = action;
    SendMessage(msg);
}

void OnlineClient::MakeMove(QPair<int, int> cell, OnlineEpochId epoch_id) {
    BoardAction action;
    action.type = BoardActionType::MOVE;
    action.coords = cell;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}

void OnlineClient::Undo(OnlineEpochId epoch_id) {
    if (!CheckTimeout()) {
        std::cerr << "too fast" << std::endl;
        return;
    }
    BoardAction action;
    action.type = BoardActionType::UNDO;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}

void OnlineClient::UndoUntil(QPair<int, int> cell, OnlineEpochId epoch_id) {
    BoardAction action;
    action.type = BoardActionType::UNDO_UNTIL;
    action.coords = cell;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}

void OnlineClient::Redo(OnlineEpochId epoch_id) {
    if (!CheckTimeout()) {
        std::cerr << "too fast" << std::endl;
        return;
    }
    BoardAction action;
    action.type = BoardActionType::REDO;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}


/// private

void OnlineClient::SocketReadyRead() {
    std::cerr << "new data on socket" << std::endl;
    *data_ += sock_->readAll();
    for (auto resp : take_new_responses(data_)) {
        if (resp.type == ResponseType::UPDATE) {
            std::cerr << "emit update" << std::endl;
            emit ReceivedUpdate(resp.action);
        }
        else if (resp.type == ResponseType::INIT) {
            emit ReceivedInit(resp.action);
        }
        else if (resp.type == ResponseType::STATUS) {
            emit ReceivedStatus(resp.status);
        }
    }
}

void OnlineClient::SendMessage(Message msg) {
    std::string bytes_str = Serialize<Message>(msg);
    sock_->write(QByteArray(bytes_str.c_str(), bytes_str.length()));
}

bool OnlineClient::CheckTimeout() {
    auto cur_time = QDateTime::currentDateTime();
    auto msecs = last_msg_time_.msecsTo(cur_time);
    last_msg_time_ = cur_time;
    return msecs > msecs_timeout_;
}
