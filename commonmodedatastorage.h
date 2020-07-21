#ifndef COMMONMODEDATASTORAGE_H
#define COMMONMODEDATASTORAGE_H

#include <QPair>
#include <QGraphicsItem>

class CommonModeDataStorage
{
public:
    CommonModeDataStorage();

    // stones in order of the position
    QVector<QGraphicsItem*> stones_pos;

    // numbers for the stones in ascending order
    QVector<QGraphicsItem*> numbers_pos;

    // lines created with "draw line"
    QVector<QGraphicsItem*> lines;
    // draw line variables
    QPair<int, int> line_point_a, line_point_b;
    QGraphicsItem *pending_line = nullptr;

    // marks of moves in the tree in current position
    QVector<QGraphicsItem*> marks;
};

#endif // COMMONMODEDATASTORAGE_H
