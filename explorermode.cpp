#include "explorermode.h"
#include "enums.h"

ExplorerModeBase::ExplorerModeBase(ExplorerMode mode, AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage) :
    mode_(mode), board_(board), painter_(painter), storage_(storage) {}

ExplorerModeBase::ExplorerModeBase(AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage) :
    ExplorerModeBase(BASE, board, painter, storage) {}

ExplorerMode ExplorerModeBase::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    return mode_;
}

ExplorerMode ExplorerModeBase::HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    return mode_;
}

ExplorerMode ExplorerModeBase::HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    return mode_;
}

ExplorerMode ExplorerModeBase::HandleKeyPressEvent(QKeyEvent *event) {
    (void)event;
    return mode_;
}

void ExplorerModeBase::RenderMarks() {
    // delete old marks
    for (auto mark : storage_->marks) {
        delete mark;
    }
    storage_->marks.clear();

    // draw new marks
    auto cur_node = board_->tree_->GetCurrentNode();
    for (auto pos : cur_node->children.keys()) {
       auto mark = painter_->DrawMoveMark(pos, board_->GetCurrentColor());
       storage_->marks.push_back(mark);
    }
}

void ExplorerModeBase::MakeMove(QPair<int, int> cell) {
    // update abstract board
    board_->MakeMove(cell);

    // draw new stone
    auto stone_txt_pair = painter_->DrawNumberedStone(cell,
                                                      opposite_color(board_->GetCurrentColor()),
                                                      board_->MovesCount());
    storage_->stones_pos.push_back(stone_txt_pair.first);
    storage_->numbers_pos.push_back(stone_txt_pair.second);

    RenderMarks();
}

void ExplorerModeBase::Undo() {
    if (!board_->Empty()) {
        board_->UndoLastMove();
        delete storage_->stones_pos.back();
        storage_->stones_pos.pop_back();
        delete storage_->numbers_pos.back();
        storage_->numbers_pos.pop_back();
        RenderMarks();
    }
}

void ExplorerModeBase::UndoUntil(QPair<int, int> cell) {
    while (!board_->Empty() && board_->GetLastMove() != cell) {
        Undo();
    }
    RenderMarks();
}


ExplorerModeDefault::ExplorerModeDefault(AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage) :
    ExplorerModeBase(DEFAULT, board, painter, storage) {}

ExplorerMode ExplorerModeDefault::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = painter_->GetCell(event->scenePos());
    if (!board_->IsCell(cell)) {
        // do nothing - its not a click on the board
        return DEFAULT;
    }
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            storage_->line_point_a = cell;
            storage_->line_point_b = cell;
            return DRAWLINE;
        }
        else {
            if (board_->GetCell(cell) == EMPTY) {
                MakeMove(cell);
            }
            else {
                UndoUntil(cell);
            }
        }
    }
    else {
        Undo();
    }
    return DEFAULT;
}


ExplorerModeDrawLine::ExplorerModeDrawLine(AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage) :
    ExplorerModeBase(DRAWLINE, board, painter, storage) {}


ExplorerMode ExplorerModeDrawLine::HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = painter_->GetCell(event->scenePos());
    if (!board_->IsCell(cell)) {
        // do nothing - its not a click on the board
        return DRAWLINE;
    }
    if (cell != storage_->line_point_b) {
        storage_->line_point_b = cell;
        if (storage_->pending_line) {
            delete storage_->pending_line;
        }
        storage_->pending_line = painter_->DrawLineAB(storage_->line_point_a, storage_->line_point_b);
    }
    return DRAWLINE;
}


ExplorerMode ExplorerModeDrawLine::HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    storage_->lines.push_back(storage_->pending_line);
    storage_->pending_line = nullptr;
    return DEFAULT;
}
