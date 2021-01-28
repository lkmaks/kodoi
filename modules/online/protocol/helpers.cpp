#ifndef HELPERS_H
#define HELPERS_H

#include "helpers.h"
#include <QString>

bool check_int(QString str, int from, int to) {
    bool ok;
    int val = str.toInt(&ok);
    return ok && from <= val && val < to;
}

bool check_int(QString str) {
    bool ok;
    str.toInt(&ok);
    return ok;
}

bool check_ulonglong(QString str) {
    bool ok;
    str.toULongLong(&ok);
    return ok;
}

#endif // HELPERS_H
