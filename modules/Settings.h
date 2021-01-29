#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>

struct Settings {
    QString name;
    QString password;
    bool is_guest;
    bool view_marks = true;
    bool view_numbers = true;
    QString engine_cmd = "engine.exe";
};


#endif // APPSETTINGS_H
