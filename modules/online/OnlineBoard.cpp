#include "OnlineBoard.h"
#include <QMutexLocker>
#include <iostream>

OnlineBoard::OnlineBoard(AbstractBoard *board)
{
    epoch_id_ = 0;
    board_ = board;
}

OnlineEpochId OnlineBoard::GetEpochId() {
    return epoch_id_;
}

void OnlineBoard::ApplyAction(BoardAction action) {
    if (action.type == BoardActionType::MOVE) {
        board_->MakeMove(action.coords);
    }
    else if (action.type == BoardActionType::REDO) {
        board_->Redo();
    }
    else if (action.type == BoardActionType::UNDO) {
        board_->Undo();
    }
    else if (action.type == BoardActionType::UNDO_UNTIL) {
        board_->UndoUntil(action.coords);
    }
    epoch_id_ = action.epoch_id;
}

AbstractBoard *OnlineBoard::GetBoard() {
    return board_;
}
