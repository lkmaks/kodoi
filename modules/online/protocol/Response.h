#ifndef RESPONSE_H
#define RESPONSE_H

#include <QByteArray>

#include "helpers.h"
#include "BoardAction.h"

enum class ResponseType {
    STATUS, // to notify if action was successful
    UPDATE, // to send updates on actions commited by any user
    INIT // to send state in the beginning
};


const int RESPONSE_LEN = 25;

struct Response
{
    ResponseType type;

    bool status; // for status

    BoardAction action; // for update, init

    Response();
    Response(bool);

    static Response Ok();
    static Response Fail();
    static Response Update(BoardAction);

    SERIALIZE(type, status, action);
};

std::vector<Response> take_new_responses(QByteArray *arr);

#endif // RESPONSE_H
