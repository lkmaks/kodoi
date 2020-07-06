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

    // objects groups
    // <stone, text>
    // stones have numbers 1, 2, ... numbered_stones.size() in text
    QVector<QGraphicsItem*> stones_buf_pos_;
    QVector<QGraphicsItem*> txt_buf_pos_;
    QVector<QGraphicsItem*> numbers_;
    QVector<QGraphicsItem*> lines_;

};

#endif // COMMONMODEDATASTORAGE_H
