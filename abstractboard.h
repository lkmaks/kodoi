#ifndef RENJUBOARD_H
#define RENJUBOARD_H

#include <QVector>
#include "config.h"
#include "explorertree.h"
#include "enums.h"

class AbstractBoard
{
public:
    AbstractBoard(const Config &config);

    QPair<int, int> GetLastMove();
    bool Empty();
    Cell GetCell(QPair<int, int> cell);
    StoneColor GetCurrentColor();
    int MovesCount();

    void MakeMove(QPair<int, int> cell);
    void UndoLastMove();

    static bool IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2);
    bool IsCell(QPair<int, int> cell);

    // logically part of the Board responsibility but has its own job and needs to be accessible
    ExplorerTree *tree_;
private:
    int board_size_;
    QVector<QVector<Cell> > board_arr_;
    QVector<QPair<int, int> > position_;

    const Config &config_;
};

#endif // RENJUBOARD_H
