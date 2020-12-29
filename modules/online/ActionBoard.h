#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <QMutex>

#include "common/AbstractBoard.h"
#include "helpers.h"


using EpochId = size_t;

enum class BoardActionType {
    MOVE,
    UNDO,
    REDO,
    UNDO_UNTIL
};

struct BoardAction {
    BoardActionType type;
    QPair<int, int> coords; // for move, undo_until
    EpochId epoch_id; // epoch to which this action is meant to apply
    SERIALIZE(type, coords.first, coords.second, epoch_id);
};

class ActionBoard
{
public:
    ActionBoard(AbstractBoard *board);
    EpochId GetEpochId();
    bool ApplyAction(BoardAction);

//    bool MakeMove(QPair<int, int> coords);
//    bool Undo();
//    bool UndoUntil(QPair<int, int> coords);
//    bool Redo();
private:
    EpochId epoch_id_;
    AbstractBoard *board_;
};

#endif // BOARD_H

