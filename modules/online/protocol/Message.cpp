#include <QString>
#include <sstream>
#include <cereal/archives/binary.hpp>

#include "Message.h"


std::vector<Message> take_new_messages(QByteArray *arr) {
    std::vector<Message> result;
    for (int i = MESSAGE_LEN; i <= arr->size(); i += MESSAGE_LEN) {
        std::string cur_str = arr->mid(i - MESSAGE_LEN, MESSAGE_LEN).toStdString();
        result.push_back(Deserialize<Message>(cur_str));
    }
    arr->remove(0, arr->size() - arr->size() % MESSAGE_LEN);
    return result;
}
