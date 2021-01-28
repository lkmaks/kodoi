#include "OnlineSession.h"
#include <QThread>


OnlineSession::OnlineSession(QObject *parent) : QObject(parent)
{
    data_ = new QByteArray();
    sock_ = new QTcpSocket();
    connect(sock_, &QTcpSocket::readyRead, this, &OnlineSession::SocketReadyRead);
    sock_->connectToHost("18.196.82.235", 12345);
    //sock_->connectToHost("127.0.0.1", 12345);
    sock_->waitForConnected();
}


void OnlineSession::Create(RoomId room_id) {
    SendMessage(Message::Create(room_id));
}

void OnlineSession::Enter(RoomId room_id) {
    SendMessage(Message::Enter(room_id));

    /// dont want to include this yet:

    // this is kind of premature, but if you use client correcly
    // (meaning do not use non-last enter), its fine
    //room_id_ = room_id;
}

void OnlineSession::MakeBoardAction(BoardAction action) {
    SendMessage(Message::Action(action));
}

void OnlineSession::MakeMove(QPair<int, int> cell, OnlineEpochId epoch_id) {
    BoardAction action;
    action.type = BoardActionType::MOVE;
    action.coords = cell;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}

void OnlineSession::Undo(OnlineEpochId epoch_id) {
    BoardAction action;
    action.type = BoardActionType::UNDO;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}

void OnlineSession::UndoUntil(QPair<int, int> cell, OnlineEpochId epoch_id) {
    BoardAction action;
    action.type = BoardActionType::UNDO_UNTIL;
    action.coords = cell;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}

void OnlineSession::Redo(OnlineEpochId epoch_id) {
    BoardAction action;
    action.type = BoardActionType::REDO;
    action.epoch_id = epoch_id;
    MakeBoardAction(action);
}


/// private

void OnlineSession::SocketReadyRead() {
    std::cerr << "new data on socket" << std::endl;
    *data_ += sock_->readAll();
    for (auto msg : Protocol::take_new_messages(data_)) {
        auto method = msg[Protocol::KEY_METHOD];
        if (method == Protocol::VALUE_METHOD_UPDATE) {
            std::cerr << "emit update" << std::endl;
            emit ReceivedUpdate(msg.GetAction());
        }
        else if (method == Protocol::VALUE_METHOD_INIT) {
            std::cerr << "emit receive" << std::endl;
            emit ReceivedInit(msg.GetAction());
        }
        else if (method == Protocol::VALUE_METHOD_STATUS) {
            std::cerr << "emit status" << std::endl;
            emit ReceivedStatus(msg.GetStatus());
        }
    }
}

void OnlineSession::SendMessage(Message msg) {
    sock_->write(Protocol::SerializeMessage(msg));
}
