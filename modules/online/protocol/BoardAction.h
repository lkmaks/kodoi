#ifndef BOARDACTION_H
#define BOARDACTION_H

#include <QPair>
#include "../types.h"

enum class BoardActionType {
    MOVE,
    UNDO,
    REDO,
    UNDO_UNTIL
};

struct BoardAction {
    BoardActionType type;
    QPair<int, int> coords; // for move, undo_until
    OnlineEpochId epoch_id; // epoch to which this action is meant to apply
};

#endif // BOARDACTION_H
