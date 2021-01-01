#ifndef BOARDACTION_H
#define BOARDACTION_H

#include <QPair>
#include "common/Enums.h"
#include "helpers.h"


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
    SERIALIZE(type, coords.first, coords.second, epoch_id);
};

#endif // BOARDACTION_H
