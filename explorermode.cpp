#include "explorermode.h"

/*

void ExplorerModeBase::AddStone(int i, int j) {
    int color = board_.GetCurrentColor();
    board_.MakeMove(i, j);
    board_painter_->putStone(i, j, color, board_.MovesCount());
}


void ExplorerModeBase::UndoUntil(int i, int j) {
    while (!board_.Empty() && board_.GetLastMove() != QPair<int, int>(i, j)) {
        undo();
    }
}


void ExplorerModeBase::Undo() {
    if (!board_.Empty()) {
        auto last_move = board_.GetLastMove();
        board_.UndoLastMove();
        delete stones.back();
        delete numbers.back();
        stones.pop_back();
        numbers.pop_back();
    }
}
*/

ExplorerMode ExplorerModeDefault::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = board_painter_->GetCell(event->scenePos());
    if (board_->IsCell(cell, cell)) {

    }
}
