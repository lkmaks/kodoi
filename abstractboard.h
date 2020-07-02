#ifndef RENJUBOARD_H
#define RENJUBOARD_H

#include <QVector>
#include "config.h"

enum StoneColor {
    WHITE,
    BLACK
};

enum Cell {
    EMPTY,
    WHITESTONE,
    BLACKSTONE
};

class AbstractBoard
{
public:
    AbstractBoard(const Config &config);

    QPair<int, int> GetLastMove();
    bool Empty();
    Cell GetCell(int i, int j);
    StoneColor GetCurrentColor();
    int MovesCount();

    void MakeMove(int i, int j);
    void UndoLastMove();

    static bool IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2);
    bool IsCell(QPair<int, int> cell);
private:
    int board_size_;
    QVector<QVector<Cell> > board_arr;
    QVector<QPair<int, int> > position;
    const Config &config_;
};

#endif // RENJUBOARD_H
