#ifndef ONLINECLIENT_H
#define ONLINECLIENT_H

#include <QPair>
#include <QObject>
#include <QTcpSocket>
#include <QDateTime>

#include "common/Enums.h"
#include "protocol/Message.h"
#include "types.h"

using Protocol::Message;

class OnlineSession : public QObject
{
    Q_OBJECT

public:
    OnlineSession(QObject *parent = nullptr);

    /// send message

    void Login(QString name, QString password);
    void LoginAsGuest();

    void CreateRoom(RoomId room_id);

    void EnterRoom(RoomId room_id);
    void NeedInit();

    void MakeBoardAction(BoardAction action);

    void LeaveRoom(RoomId room_id);
    void RoomsList();


    /// convinience
    void MakeMove(QPair<int, int> cell, OnlineEpochId epoch_id);
    void Undo(OnlineEpochId epoch_id);
    void UndoUntil(QPair<int, int> cell, OnlineEpochId epoch_id);
    void Redo(OnlineEpochId epoch_id);

signals:
    // signals about receiveing messages
    void ReceivedRoomAdded(QString name);
    void ReceivedStatus(QString status);
    void ReceivedGuestName(QString name);
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
