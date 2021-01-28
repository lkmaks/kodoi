#ifndef BOARD_H
#define BOARD_H

#include "common/AbstractBoard.h"
#include "../types.h"
#include "../protocol/BoardAction.h"

class OnlineBoard
{
public:
    OnlineBoard(AbstractBoard *board);
    OnlineEpochId GetEpochId();
    void ApplyAction(BoardAction action);

    AbstractBoard *GetBoard();
private:
    OnlineEpochId epoch_id_;
    AbstractBoard *board_;
};

#endif // BOARD_H
