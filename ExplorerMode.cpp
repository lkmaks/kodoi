#include "ExplorerMode.h"
#include "Enums.h"
#include <QDebug>
#include <QEventLoop>
#include "Enums.h"

class MainWidget;

ExplorerModeBase::ExplorerModeBase(ExplorerMode mode, BoardTools tools) :
    mode_(mode), config_(tools.config), settings_(tools.settings), board_(tools.board),
    painter_(tools.painter), storage_(tools.storage), engine_wrapper_(tools.engine_wrapper) {}

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
    UpdatePonderingPosition();
}

void ExplorerModeBase::Undo() {
    bool succ = board_->Undo();
    if (succ) {
        // really commiting undo
        delete storage_->stones_pos.back();
        storage_->stones_pos.pop_back();
        delete storage_->numbers_pos.back();
        storage_->numbers_pos.pop_back();
        RenderMarks();
        UpdatePonderingPosition();
    }
}

void ExplorerModeBase::UndoUntil(QPair<int, int> cell) {
    while (!board_->Empty() && board_->GetLastMove() != cell) {
        Undo();
    }
    RenderMarks();
    UpdatePonderingPosition();
}

void ExplorerModeBase::Redo() {
    // update abstract board
    bool succ = board_->Redo();

    if (succ) {
        // draw new stone
        auto cell = board_->GetLastMove();
        auto stone_txt_pair = painter_->DrawNumberedStone(cell,
                                                          opposite_color(board_->GetCurrentColor()),
                                                          board_->MovesCount());
        storage_->stones_pos.push_back(stone_txt_pair.first);
        storage_->numbers_pos.push_back(stone_txt_pair.second);

        RenderMarks();
        UpdatePonderingPosition();
    }
}

void ExplorerModeBase::StartPondering() {
    storage_->engine_state = EngineState::STARTING;
    engine_wrapper_->Start();

    QEventLoop loop;
    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStarted, &loop, &QEventLoop::quit);
    QObject::connect(engine_wrapper_, &EngineWrapper::ErrorOccured, &loop, &QEventLoop::quit);
    loop.exec();

    if (engine_wrapper_->GetBrainProcessState() != QProcess::ProcessState::Running) {
        storage_->engine_state = EngineState::FAILED;
        return;
    }

    storage_->engine_state = EngineState::ACTIVE;
    engine_wrapper_->Setup({});
    engine_wrapper_->StartThinking(board_->GetEngineFormatPosition(), 5);
    storage_->pondering_epoch_id = 1; // starting each time with epoch 1, same as engine will
}

void ExplorerModeBase::UpdatePonderingPosition() {
    if (storage_->engine_state == EngineState::ACTIVE) {
        engine_wrapper_->StopThinking();
        engine_wrapper_->StartThinking(board_->GetEngineFormatPosition(), 5);
        ++storage_->pondering_epoch_id;
    }
}

void ExplorerModeBase::StopPondering() {
    storage_->engine_state = EngineState::STOPPING;
    engine_wrapper_->StopThinking();
    engine_wrapper_->ForceStop();
    storage_->engine_state = EngineState::STOPPED;
}

//void ExplorerModeBase::SetViewMarks(bool view) {
//    settings_->view_marks = view;
//    if (view) {
//        for (auto mark : storage_->marks) {
//            // TODO: unhide
//        }
//    }
//}

ExplorerModeDefault::ExplorerModeDefault(BoardTools tools)
    : ExplorerModeBase(ExplorerMode::DEFAULT, tools) {}

ExplorerMode ExplorerModeDefault::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = painter_->GetCell(event->scenePos());
    if (!board_->IsCell(cell)) {
        // do nothing - its not a click on the board
        return ExplorerMode::DEFAULT;
    }
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            storage_->line_point_a = cell;
            storage_->line_point_b = cell;
            return ExplorerMode::DRAWLINE;
        }
        else {
            if (board_->GetCell(cell) == Cell::EMPTY) {
                MakeMove(cell);
            }
            else {
                UndoUntil(cell);
            }
        }
    }
    else if (event->button() == Qt::RightButton) {
        Undo();
    }
    return ExplorerMode::DEFAULT;
}


ExplorerMode ExplorerModeDefault::HandleKeyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_C) {
        // clear temporary objects
        for (auto line : storage_->lines) {
            delete line;
        }
        storage_->lines.clear();
    }
    else if (event->key() == Qt::Key_Left) {
        Undo();
    }
    else if (event->key() == Qt::Key_Right) {
        Redo();
    }
    else if (event->key() == Qt::Key_P) {
        //qDebug() << "pressed P" << endl;
        if (storage_->engine_state == EngineState::STOPPED) {
            StartPondering();
        }
        else if (storage_->engine_state == EngineState::ACTIVE) {
            StopPondering();
        }
    }
    return ExplorerMode::DEFAULT;
}


ExplorerModeDrawLine::ExplorerModeDrawLine(BoardTools tools)
    : ExplorerModeBase(ExplorerMode::DRAWLINE, tools) {}


ExplorerMode ExplorerModeDrawLine::HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = painter_->GetCell(event->scenePos());
    if (!board_->IsCell(cell)) {
        // do nothing - its not a click on the board
        return ExplorerMode::DRAWLINE;
    }
    if (cell != storage_->line_point_b) {
        storage_->line_point_b = cell;
        if (storage_->pending_line) {
            delete storage_->pending_line;
        }
        qDebug() << storage_->line_point_a << "  " << storage_->line_point_b << "\n";
        storage_->pending_line = painter_->DrawLineAB(storage_->line_point_a, storage_->line_point_b);
    }
    return ExplorerMode::DRAWLINE;
}


ExplorerMode ExplorerModeDrawLine::HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    storage_->lines.push_back(storage_->pending_line);
    storage_->pending_line = nullptr;
    return ExplorerMode::DEFAULT;
}
