#ifndef ONLINECLIENT_H
#define ONLINECLIENT_H

#include <QPair>
#include <QObject>
#include <QTcpSocket>
#include <QDateTime>

#include "common/Enums.h"
#include "protocol/BoardAction.h"
#include "protocol/Message.h"


class OnlineClient : public QObject
{
    Q_OBJECT

public:
    OnlineClient(QObject *parent = nullptr);

    void SetRoomId(RoomId room_id);

    // messages types
    void Create(RoomId room_id);
    void Enter(RoomId room_id);
    void MakeBoardAction(BoardAction action);

    // convinience
    void MakeMove(QPair<int, int> cell, OnlineEpochId epoch_id);
    void Undo(OnlineEpochId epoch_id);
    void UndoUntil(QPair<int, int> cell, OnlineEpochId epoch_id);
    void Redo(OnlineEpochId epoch_id);

signals:
    void ReceivedStatus(bool status);
    void ReceivedInit(BoardAction action);
    void ReceivedUpdate(BoardAction action);
private:

    // slot
    void SocketReadyRead();

    // low level to send commands
    void SendMessage(Message msg);

    QTcpSocket *sock_;
    QByteArray *data_;
    RoomId room_id_;

    // kostyl
    bool CheckTimeout();
    QDateTime last_msg_time_;
    int msecs_timeout_ = 50;
};

#endif // ONLINECLIENT_H
