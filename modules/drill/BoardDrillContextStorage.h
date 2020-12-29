#ifndef BOARDDRILLCONTEXTSTORAGE_H
#define BOARDDRILLCONTEXTSTORAGE_H

#include <QPair>
#include <QGraphicsItem>
#include "common/Enums.h"
#include "Config.h"

struct BoardDrillContextStorage
{
    BoardDrillContextStorage();

    // stones in order of the position
    QVector<QGraphicsItem*> stones_pos;

    // numbers for the stones in ascending order
    QVector<QGraphicsItem*> numbers_pos;

    QGraphicsItem *last_stone_border_highlight = nullptr;

    // marks of moves in the tree in current position
    QVector<QGraphicsItem*> marks;

    bool view_marks = true;
    bool view_last_move_highlight = true;
};


#endif // BOARDDRILLCONTEXTSTORAGE_H
