#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPair>
#include "Config.h"
#include "BoardScene.h"
#include "AbstractBoard.h"

class BoardPainter
{
public:
    BoardPainter(const Config *config, BoardScene *board_scene);

    void PaintEmptyBoard();

    QPair<int, int> GetCell(QPointF pos);

    QPair<QGraphicsItem*, QGraphicsItem*> DrawNumberedStone(QPair<int, int> cell, StoneColor color, int number);
    QGraphicsItem *DrawLineAB(QPair<int, int> a, QPair<int, int> b);
    QGraphicsItem *DrawMoveMark(QPair<int, int> cell, StoneColor color);
    QGraphicsItem *DrawEvalCircle(QPair<int, int> cell, QColor color);
    QGraphicsItem *HighlightStoneBorder(QPair<int, int> cell, QColor color);

    void RemoveItem(QGraphicsItem *item);

private:
    const Config *config_;
    BoardScene *board_scene_;

    // parameters of proportions
    int cell_size_;
    int board_size_;
    qreal stone_gap_factor_ = 0;
};

#endif // BOARDPAINTER_H
