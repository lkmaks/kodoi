#ifndef ENUMS_H
#define ENUMS_H

#include <vector>

enum class StoneColor {
    WHITE,
    BLACK,
    NONE
};

StoneColor opposite_color(StoneColor color);

enum class CellType {
    EMPTY,
    WHITESTONE,
    BLACKSTONE
};

enum class Rule {
    FREESTYLE,
    GOMOKU,
    RENJU
};

enum class EngineState {
    STOPPED,
    STARTING,
    ACTIVE,
    STOPPING,
    FAILED
};

enum class SettingsField {
    VIEW_MARKS,
    VIEW_NUMBERS,
    ENGINE_CMD
};

using EngineEpochId = int;
using OnlineEpochId = size_t;

#endif // ENUMS_H
