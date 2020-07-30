#ifndef RENJUBOARD_H
#define RENJUBOARD_H

#include <QVector>
#include "Config.h"
#include "ExplorerTree.h"
#include "Enums.h"

class AbstractBoard
{
public:
    AbstractBoard(const Config *config);

    QPair<int, int> GetLastMove();
    bool Empty();
    Cell GetCell(QPair<int, int> cell);
    StoneColor GetCurrentColor();
    int MovesCount();

    void MakeMove(QPair<int, int> cell);
    bool Undo();
    bool Redo();

    static bool IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2);
    bool IsCell(QPair<int, int> cell);

    // logically part of the Board responsibility but has its own job and needs to be accessible
    ExplorerTree *tree_;
private:
    int board_size_;
    QVector<QVector<Cell> > board_arr_;
    QVector<QPair<int, int> > sequence_;
    int cur_seq_len_;

    const Config *config_;
};

#endif // RENJUBOARD_H
