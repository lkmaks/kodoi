#ifndef ONLINECLIENT_H
#define ONLINECLIENT_H

#include <QPair>
#include <QObject>
#include <QTcpSocket>

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
    // better in terms of copy-paste
    void ReceivedStatus(bool status);
    void ReceivedInit(BoardAction action);
    void ReceivedUpdate(BoardAction action);
//    void ReceivedMakeMove(QPair<int, int> cell, OnlineEpochId epoch_id);
//    void RecievedUndo(OnlineEpochId epoch_id);
//    void RecievedUndoUntil(QPair<int, int> cell, OnlineEpochId epoch_id);
//    void RecievedRedo(OnlineEpochId epoch_id);
private:

    // slot
    void SocketReadyRead();

    // low level to send commands
    void SendMessage(Message msg);

    QTcpSocket *sock_;
    QByteArray *data_;
    RoomId room_id_;
};

#endif // ONLINECLIENT_H
