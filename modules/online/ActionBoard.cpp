#include "ActionBoard.h"

ActionBoard::ActionBoard(AbstractBoard *board)
{
    epoch_id_ = 0;
    board_ = board;
}

EpochId ActionBoard::GetEpochId() {
    return epoch_id_;
}

bool ActionBoard::ApplyAction(BoardAction action) {
    // assume epoch matches board epoch

    bool ok = false;

    if (action.type == BoardActionType::MOVE) {
        ok = board_->MakeMove(action.coords);
    }
    else if (action.type == BoardActionType::REDO) {
        ok = board_->Redo();
    }
    else if (action.type == BoardActionType::UNDO) {
        ok = board_->Undo();
    }
    else if (action.type == BoardActionType::UNDO_UNTIL) {
        ok = board_->UndoUntil(action.coords);
    }

    if (ok) {
        ++epoch_id_;
    }

    return ok;
}

//bool Board::MakeMove(QPair<int, int> coords) {
//    return board_->MakeMove(coords);
//}

//bool Board::UndoUntil(QPair<int, int> coords) {
//    return board_->UndoUntil(coords);
//}

//bool Board::Undo() {
//    return board_->Undo();
//}

//bool Board::Redo() {
//    return board_->Redo();
//}

