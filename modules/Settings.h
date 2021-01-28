#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>

struct Settings {
    bool view_marks = true;
    bool view_numbers = true;
    QString engine_cmd = "engine.exe";
};


#endif // APPSETTINGS_H
