#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <vector>
#include <map>


#include "types.h"
#include "serialization.h"
#include "BoardAction.h"


namespace Protocol {
    using MessageSizeType = int;
    using Key = QString;
    using Value = QString;
    using Dict = std::map<Key, Value>;


    const Key KEY_METHOD = "method";

    const Value VALUE_METHOD_STATUS = "status";
    const Value VALUE_METHOD_INIT = "init";
    const Value VALUE_METHOD_UPDATE = "update";
    const Value VALUE_METHOD_USER_ENTERED = "user_entered";
    const Value VALUE_METHOD_USER_LEFT = "user_left";

    const Value VALUE_METHOD_LOGIN = "login";
    const Value VALUE_METHOD_ROOMS_LIST = "list";
    const Value VALUE_METHOD_CREATE = "create";
    const Value VALUE_METHOD_ENTER = "enter";
    const Value VALUE_METHOD_LEAVE = "leave";
    const Value VALUE_METHOD_ACTION = "action";


    const Key KEY_STATUS = "status";
    const Value VALUE_STATUS_OK = "ok";
    const Value VALUE_STATUS_FAIL = "fail";

    const Key KEY_ACTION_TYPE = "action_type";
    const Key KEY_ACTION_EPOCH_ID = "action_epoch";
    const Key KEY_ACTION_COORD_1 = "action_x";
    const Key KEY_ACTION_COORD_2 = "action_y";

    const Key KEY_ROOM_ID = "room_id";

    const Key KEY_LOGIN_NAME = "login_name";
    const Key KEY_LOGIN_PASSWORD = "login_password";

    const Key KEY_USER_ENTER_NAME = "user_enter_name";
    const Key KEY_USER_LEAVE_NAME = "user_leave_name";

    class Message {
    public:
        Message(std::map<Key, Value> dict_ = {});

        bool has(Key key);
        Value &operator[](Key key);


        /// convinience constructors

        // for server
        static Message Status(bool status);
        static Message Ok();
        static Message Fail();
        static Message Init(BoardAction action);
        static Message Update(BoardAction action);
        static Message UserEntered(QString name);
        static Message UserLeft(QString name);

        // for client
        static Message Login(QString login, QString password);
        static Message RoomsList();
        static Message Create(RoomId room_id);
        static Message Enter(RoomId room_id);
        static Message Leave(RoomId room_id);
        static Message Action(BoardAction);

        /// convinience retrievers

        BoardAction GetAction();
        QString GetStatus();

        bool IsCorrect();

        SERIALIZE(dict_);
    private:
        Dict dict_;

        static Message ActionMessage(BoardAction action);

        bool ContainsCorrectAction();
    };

    QByteArray SerializeMessage(const Message &mes);
    std::vector<Message> take_new_messages(QByteArray *arr);
}



#endif // MESSAGE_H
