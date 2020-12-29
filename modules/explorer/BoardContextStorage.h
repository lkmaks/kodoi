#ifndef COMMONMODEDATASTORAGE_H
#define COMMONMODEDATASTORAGE_H

#include <QPair>
#include <QGraphicsItem>
#include "common/Enums.h"
#include "Config.h"

/// todo: remove config

class BoardContextStorage
{
public:
    BoardContextStorage(const Config *config);

    const Config *config_;

    // stones in order of the position
    QVector<QGraphicsItem*> stones_pos;

    // numbers for the stones in ascending order
    QVector<QGraphicsItem*> numbers_pos;

    QGraphicsItem *last_stone_border_highlight = nullptr;

    // lines created with "draw line"
    QVector<QGraphicsItem*> lines;
    // draw line variables
    QPair<int, int> line_point_a, line_point_b;
    QGraphicsItem *pending_line = nullptr;

    // marks of moves in the tree in current position
    QVector<QGraphicsItem*> marks;

    bool view_marks = true;
    bool view_last_move_highlight = true;

    // engine context
    EngineState engine_state = EngineState::STOPPED;
    EngineEpochId pondering_epoch_id = 0;
    QVector<QGraphicsItem*> eval_circles;
    int nbest_value = 3;
};

#endif // COMMONMODEDATASTORAGE_H
