#ifndef COMMONMODEDATASTORAGE_H
#define COMMONMODEDATASTORAGE_H

#include <QPair>
#include <QGraphicsItem>

class CommonModeDataStorage
{
public:
    CommonModeDataStorage();

    int step = 0;
    QPair<int, int> mousedown_pos;
    QPair<int, int> line_point_a, line_point_b;
    QGraphicsItem *pending_line = nullptr;
};

#endif // COMMONMODEDATASTORAGE_H
