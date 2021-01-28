#ifndef ONLINECLIENT_H
#define ONLINECLIENT_H

#include <QPair>
#include <QObject>
#include <QTcpSocket>
#include <QDateTime>

#include "common/Enums.h"
#include "protocol/Message.h"

using Protocol::Message;

class OnlineSession : public QObject
{
    Q_OBJECT

public:
    OnlineSession(QObject *parent = nullptr);

    // send message
    void Create(RoomId room_id);
    void Enter(RoomId room_id);
    void MakeBoardAction(BoardAction action);

    // convinience
    void MakeMove(QPair<int, int> cell, OnlineEpochId epoch_id);
    void Undo(OnlineEpochId epoch_id);
    void UndoUntil(QPair<int, int> cell, OnlineEpochId epoch_id);
    void Redo(OnlineEpochId epoch_id);

signals:
    // signals about receiveing messages
    void ReceivedStatus(QString status);
    void ReceivedInit(BoardAction action);
    void ReceivedUpdate(BoardAction action);
private:

    // slot
    void SocketReadyRead();

    // low level method to send messages
    void SendMessage(Message msg);

    QTcpSocket *sock_;
    QByteArray *data_;
};

#endif // ONLINECLIENT_H
