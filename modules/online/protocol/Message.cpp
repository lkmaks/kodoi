#include <QString>
#include <sstream>

#include "Message.h"

#include "serialization.h"
#include "helpers.h"


namespace Protocol {
    const int message_pref_len = sizeof(MessageSizeType);

    Message::Message(std::map<Key, Value> dict) : dict_(dict) {}

    /// normal methods

    bool Message::has(Key key) {
        return dict_.count(key);
    }

    Value &Message::operator[](Key key) {
        return dict_[key];
    }


    /// constructors

    // server

    Message Message::Status(bool status) {
        return Message({{KEY_METHOD, VALUE_METHOD_STATUS},
                        {KEY_STATUS, (status ? VALUE_STATUS_OK : VALUE_STATUS_FAIL)}});
    }

    Message Message::Ok() {
        return Message::Status(true);
    }

    Message Message::Fail() {
        return Message::Status(false);
    }

    Message Message::Init(BoardAction action) {
        auto msg = ActionMessage(action);
        msg[KEY_METHOD] = VALUE_METHOD_INIT;
        return msg;
    }

    Message Message::Update(BoardAction action) {
        auto msg = ActionMessage(action);
        msg[KEY_METHOD] = VALUE_METHOD_UPDATE;
        return msg;
    }

    Message Message::UserEntered(QString name) {
        return Message({{KEY_METHOD, VALUE_METHOD_USER_ENTERED}, {KEY_USER_ENTER_NAME, name}});
    }

    Message Message::UserLeft(QString name) {
        return Message({{KEY_METHOD, VALUE_METHOD_USER_LEFT}, {KEY_USER_LEAVE_NAME, name}});
    }

    Message Message::RoomAdded(QString name) {
        return Message({{KEY_METHOD, VALUE_METHOD_ROOM_ADDED}, {KEY_ROOM_ID, name}});
    }

    // client

    Message Message::Login(QString name, QString password) {
        return Message({{KEY_METHOD, VALUE_METHOD_LOGIN}, {KEY_LOGIN_NAME, name}, {KEY_LOGIN_PASSWORD, password}});
    }

    Message Message::RoomsList() {
        return Message(Dict({{KEY_METHOD, VALUE_METHOD_ROOMS_LIST}}));
    }

    Message Message::CreateRoom(RoomId room_id) {
        return Message({{KEY_METHOD, VALUE_METHOD_CREATE},
                        {KEY_ROOM_ID, room_id}});
    }

    Message Message::Enter(RoomId room_id) {
        return Message({{KEY_METHOD, VALUE_METHOD_ENTER},
                        {KEY_ROOM_ID, room_id}});
    }

    Message Message::Leave(RoomId room_id) {
        return Message({{KEY_METHOD, VALUE_METHOD_LEAVE},
                        {KEY_ROOM_ID, room_id}});
    }

    Message Message::Action(BoardAction action) {
        Message msg = ActionMessage(action);
        msg[KEY_METHOD] = VALUE_METHOD_ACTION;
        return msg;
    }

    Message Message::NeedInit() {
        return Message(Dict({{KEY_METHOD, VALUE_METHOD_NEED_INIT}}));
    }


    /// retrievers

    BoardAction Message::GetAction() {
        BoardAction res;
        res.type = static_cast<BoardActionType>(dict_[KEY_ACTION_TYPE].toInt());
        res.coords.first = dict_[KEY_ACTION_COORD_1].toInt();
        res.coords.second = dict_[KEY_ACTION_COORD_2].toInt();
        res.epoch_id = dict_[KEY_ACTION_EPOCH_ID].toULongLong(); // 32 bit ok?
        return res;
    }

    QString Message::GetRoomId() {
        return dict_[KEY_ROOM_ID];
    }

    QString Message::GetStatus() {
        return dict_[KEY_STATUS];
    }


    /// corectness checker


    bool Message::IsCorrect() {
        if (!has(KEY_METHOD)) {
            return false;
        }
        auto method = dict_[KEY_METHOD];
        if (method == VALUE_METHOD_STATUS) {
            return has(KEY_STATUS) && (dict_[KEY_STATUS] == VALUE_STATUS_OK || dict_[KEY_STATUS] == VALUE_STATUS_FAIL);
        }
        else if (method == VALUE_METHOD_UPDATE || method == VALUE_METHOD_INIT) {
            return ContainsCorrectAction();
        }
        /// TODO: create, enter, action
        return true;
    }


    /// private

    Message Message::ActionMessage(BoardAction action) {
        Message msg;
        msg[KEY_ACTION_TYPE] = QString::number(static_cast<int>(action.type));
        msg[KEY_ACTION_COORD_1] = QString::number(action.coords.first);
        msg[KEY_ACTION_COORD_2] = QString::number(action.coords.second);
        msg[KEY_ACTION_EPOCH_ID] = QString::number(action.epoch_id);
        return msg;
    }


    bool Message::ContainsCorrectAction() {
        if (!(has(KEY_ACTION_TYPE) &&
              has(KEY_ACTION_EPOCH_ID) &&
              has(KEY_ACTION_COORD_1) &&
              has(KEY_ACTION_COORD_2))) {
            return false;
        }

        if (!check_int(dict_[KEY_ACTION_TYPE], 0, 4) ||
            !check_ulonglong(dict_[KEY_ACTION_EPOCH_ID]) ||
            !check_int(dict_[KEY_ACTION_COORD_1]) ||
            !check_int(dict_[KEY_ACTION_COORD_2])) {
            return false;
        }

        return true;
    }


    /// serialization / deserialization functions

    QByteArray SerializeMessage(const Message &mes) {
        QByteArray buf = Serialize<Message>(mes);
        QByteArray size_buf = SerializeBinary<MessageSizeType>(static_cast<MessageSizeType>(buf.size()));
        return size_buf + buf;
    }


    std::vector<Message> take_new_messages(QByteArray *arr) {
        std::vector<Message> result;
        int ptr = 0;
        while (ptr + (int)sizeof(MessageSizeType) <= (int)arr->size()) {
            MessageSizeType map_size = DeserializeBinary<MessageSizeType>(arr->mid(ptr, message_pref_len));
            if (ptr + message_pref_len + map_size > arr->size()) {
                break;
            }

            try {
                Message new_msg = Deserialize<Message>(arr->mid(ptr + message_pref_len, map_size));
                result.push_back(new_msg);
            } catch (std::exception e) {
                std::cerr << "incoming message exception: " << e.what() << std::endl;
            }

            ptr += message_pref_len + map_size;
        }
        *arr = arr->right(arr->size() - ptr);
        return result;
    }
}
