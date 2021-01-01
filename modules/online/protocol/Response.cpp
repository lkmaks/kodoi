#include "Response.h"

Response::Response() {}

Response::Response(bool ok) : type(ResponseType::STATUS), status(ok) {}

Response Response::Ok() {
    return Response(true);
}

Response Response::Fail() {
    return Response(false);
}

Response Response::Update(BoardAction action) {
    Response res;
    res.type = ResponseType::UPDATE;
    res.action = action;
    return res;
}


std::vector<Response> take_new_responses(QByteArray *arr) {
    std::vector<Response> result;
    for (int i = RESPONSE_LEN; i <= arr->size(); i += RESPONSE_LEN) {
        std::string cur_str = arr->mid(i - RESPONSE_LEN, RESPONSE_LEN).toStdString();
        result.push_back(Deserialize<Response>(cur_str));
    }
    arr->remove(0, arr->size() - arr->size() % RESPONSE_LEN);
    return result;
}

