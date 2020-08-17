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
    RENJU,
    GOMOKU,
    FREESTYLE
};

#endif // ENUMS_H
