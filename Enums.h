#ifndef ENUMS_H
#define ENUMS_H

enum class StoneColor {
    WHITE,
    BLACK,
    NONE
};

StoneColor opposite_color(StoneColor color);

enum class Cell {
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
    STOPPING
};

#endif // ENUMS_H
