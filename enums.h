#ifndef ENUMS_H
#define ENUMS_H

enum StoneColor {
    WHITE,
    BLACK,
    NONE
};

StoneColor opposite_color(StoneColor color);

enum Cell {
    EMPTY,
    WHITESTONE,
    BLACKSTONE
};

#endif // ENUMS_H
