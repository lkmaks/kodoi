#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPair>
#include "config.h"
#include "boardscene.h"
#include "abstractboard.h"

class BoardPainter
{
public:
    BoardPainter(const Config &config, BoardScene *board_scene);

    void PaintEmptyBoard();
    QPair<QGraphicsItem*, QGraphicsItem*> putStone(QPair<int, int> cell, StoneColor color, int number);
    QGraphicsItem *DrawLineAB(QPair<int, int> a, QPair<int, int> b);
    QPair<int, int> GetCell(QPointF pos);

private:
    const Config &config_;
    BoardScene *board_scene_;

    // parameters of proportions
    int cell_size_;
    int board_size_;
    qreal stone_gap_factor_ = 0;
};

#endif // BOARDPAINTER_H
