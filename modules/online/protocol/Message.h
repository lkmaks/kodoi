#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <vector>

#include "helpers.h"
#include "types.h"
#include "BoardAction.h"

enum class MessageType {
    CREATE,
    ENTER,
    ACTION
};

const int MESSAGE_LEN = 28;

struct Message
{
    MessageType type;

    RoomId room_id; // for ENTER / CREATE

    BoardAction action; // for ACTION

    SERIALIZE(type, room_id, action);
};

std::vector<Message> take_new_messages(QByteArray *arr);

#endif // MESSAGE_H
