#ifndef RENJUBOARD_H
#define RENJUBOARD_H

#include <QVector>
#include "Config.h"
#include "ExplorerTree.h"
#include "Enums.h"
#include "EngineWrapper.h"


class AbstractBoard
{
public:
    AbstractBoard(int board_size = 15);

    QPair<int, int> GetLastMove();
    bool Empty();
    CellType GetCell(QPair<int, int> cell);
    StoneColor GetCurrentColor();
    int MovesCount();
    EngineWrapper::Position GetEngineFormatPosition();

    bool MakeMove(QPair<int, int> cell);
    bool Undo();
    bool UndoUntil(QPair<int, int> cell);
    bool Redo();

    static bool IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2);
    bool IsCell(QPair<int, int> cell);

    ExplorerTree *GetTree();

private:
    int board_size_;
    QVector<QVector<CellType> > board_arr_;
    QVector<QPair<int, int> > sequence_; // position up to "going back" point
    int cur_seq_len_; // prefix of sequence currently on the board

    // logically part of the Board responsibility but has its own job and needs to be accessible
    ExplorerTree *tree_;
};

#endif // RENJUBOARD_H
