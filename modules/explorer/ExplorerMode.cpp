#include "ExplorerMode.h"
#include <QDebug>
#include <QEventLoop>


ExplorerModeBase::ExplorerModeBase(ExplorerMode mode, BoardExplorerTools tools) :
    mode_(mode), tools_(tools) {
}


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

ExplorerMode ExplorerModeBase::NbestValueChanged(int new_value) {
    if (tools_.storage->engine_state == EngineState::ACTIVE) {
        tools_.storage->nbest_value = new_value;
        UpdatePonderingPosition(new_value);
    }
    return mode_;
}

void ExplorerModeBase::MakeMove(QPair<int, int> cell) {
    // update abstract board
    tools_.board->MakeMove(cell);

    // draw new stone
    auto stone_txt_pair = tools_.painter->DrawNumberedStone(cell,
                                                      opposite_color(tools_.board->GetCurrentColor()),
                                                      tools_.board->MovesCount());
    tools_.storage->stones_pos.push_back(stone_txt_pair.first);
    tools_.storage->numbers_pos.push_back(stone_txt_pair.second);

    RenderAuxiliary();
    UpdatePonderingPosition(tools_.storage->nbest_value);
}

void ExplorerModeBase::Undo() {
    bool succ = tools_.board->Undo();
    if (succ) {
        // really commiting undo
        delete tools_.storage->stones_pos.back();
        tools_.storage->stones_pos.pop_back();
        delete tools_.storage->numbers_pos.back();
        tools_.storage->numbers_pos.pop_back();
        RenderAuxiliary();
        UpdatePonderingPosition(tools_.storage->nbest_value);
    }
}

void ExplorerModeBase::UndoUntil(QPair<int, int> cell) {
    while (!tools_.board->Empty() && tools_.board->GetLastMove() != cell) {
        Undo();
    }
    RenderMarks();
    UpdatePonderingPosition(tools_.storage->nbest_value);
}

void ExplorerModeBase::Redo() {
    // update abstract board
    bool succ = tools_.board->Redo();

    if (succ) {
        // draw new stone
        auto cell = tools_.board->GetLastMove();
        auto stone_txt_pair = tools_.painter->DrawNumberedStone(cell,
                                                          opposite_color(tools_.board->GetCurrentColor()),
                                                          tools_.board->MovesCount());
        tools_.storage->stones_pos.push_back(stone_txt_pair.first);
        tools_.storage->numbers_pos.push_back(stone_txt_pair.second);

        RenderAuxiliary();
        UpdatePonderingPosition(tools_.storage->nbest_value);
    }
}


void ExplorerModeBase::RenderMarks() {
    // update current show of view marks according to
    // policy of viewing them (true/false)
    // and actual current marks for current node

    // delete old marks
    for (auto mark : tools_.storage->marks) {
        delete mark;
    }
    tools_.storage->marks.clear();

    if (tools_.storage->view_marks) {
        // draw new marks
        auto cur_node = tools_.board->GetTree()->GetCurrentNode();
        for (auto pos : cur_node->children.keys()) {
           auto mark = tools_.painter->DrawMoveMark(pos, tools_.board->GetCurrentColor());
           tools_.storage->marks.push_back(mark);
        }
    }
}

void ExplorerModeBase::RenderLastMoveHighlight() {
    if (tools_.storage->last_stone_border_highlight != nullptr) {
        tools_.painter->RemoveItem(tools_.storage->last_stone_border_highlight);
        tools_.storage->last_stone_border_highlight = nullptr;
    }
    if (!tools_.board->Empty() && tools_.storage->view_last_move_highlight) {
        tools_.storage->last_stone_border_highlight = tools_.painter->HighlightStoneBorder(tools_.board->GetLastMove(), Qt::red);
    }
}

void ExplorerModeBase::RenderAuxiliary() {
    RenderMarks();
    RenderLastMoveHighlight();
}

void ExplorerModeBase::SetViewMarks(bool show) {
    tools_.storage->view_marks = show;
    RenderMarks();
}

void ExplorerModeBase::SetLastMoveHighlight(bool show) {
    tools_.storage->view_last_move_highlight = show;
    RenderLastMoveHighlight();
}

void ExplorerModeBase::StartPondering() {
    tools_.storage->engine_state = EngineState::STARTING;
    tools_.engine_wrapper->Start();

    QEventLoop loop;
    QObject::connect(tools_.engine_wrapper, &EngineWrapper::EngineStarted, &loop, &QEventLoop::quit);
    QObject::connect(tools_.engine_wrapper, &EngineWrapper::ErrorOccured, &loop, &QEventLoop::quit);

    if (tools_.engine_wrapper->GetBrainProcessState() == QProcess::ProcessState::Starting) {
        loop.exec();
    }

    if (tools_.engine_wrapper->GetBrainProcessState() != QProcess::ProcessState::Running) {
        tools_.storage->engine_state = EngineState::FAILED;
        return;
    }

    tools_.storage->engine_state = EngineState::ACTIVE;
    tools_.engine_wrapper->Setup({});
    tools_.engine_wrapper->StartThinking(tools_.board->GetEngineFormatPosition(), tools_.storage->nbest_value);
    tools_.storage->pondering_epoch_id = 1; // starting each time with epoch 1, same as engine will
}

void ExplorerModeBase::UpdatePonderingPosition(int nbest_value) {
    if (tools_.storage->engine_state == EngineState::ACTIVE) {
        tools_.engine_wrapper->StopThinking();
        tools_.engine_wrapper->StartThinking(tools_.board->GetEngineFormatPosition(), nbest_value);
        ++tools_.storage->pondering_epoch_id;
    }
}

void ExplorerModeBase::StopPondering() {
    tools_.storage->engine_state = EngineState::STOPPING;
    tools_.engine_wrapper->StopThinking();
    tools_.engine_wrapper->ForceStop();
    tools_.storage->engine_state = EngineState::STOPPED;
}

//void ExplorerModeBase::SetViewMarks(bool view) {
//    settings_->view_marks = view;
//    if (view) {
//        for (auto mark : tools_.storage->marks) {
//            // TODO: unhide
//        }
//    }
//}

ExplorerModeDefault::ExplorerModeDefault(BoardExplorerTools tools)
    : ExplorerModeBase(ExplorerMode::DEFAULT, tools) {}

ExplorerMode ExplorerModeDefault::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = tools_.painter->GetCell(event->scenePos());
    if (!tools_.board->IsCell(cell)) {
        // do nothing - its not a click on the board
        return ExplorerMode::DEFAULT;
    }
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            tools_.storage->line_point_a = cell;
            tools_.storage->line_point_b = cell;
            return ExplorerMode::DRAWLINE;
        }
        else {
            if (tools_.board->GetCell(cell) == CellType::EMPTY) {
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
        for (auto line : tools_.storage->lines) {
            delete line;
        }
        tools_.storage->lines.clear();
    }
    else if (event->key() == Qt::Key_Left) {
        Undo();
    }
    else if (event->key() == Qt::Key_Right) {
        Redo();
    }
    else if (event->key() == Qt::Key_P) {
        //qDebug() << "pressed P" << endl;
        if (tools_.storage->engine_state == EngineState::STOPPED) {
            StartPondering();
        }
        else if (tools_.storage->engine_state == EngineState::ACTIVE) {
            StopPondering();
        }
    }
    else if (event->key() == Qt::Key_M) {
        SetViewMarks(!tools_.storage->view_marks);
    }
    else if (event->key() == Qt::Key_L) {
        SetLastMoveHighlight(!tools_.storage->view_last_move_highlight);
    }
    return ExplorerMode::DEFAULT;
}


ExplorerModeDrawLine::ExplorerModeDrawLine(BoardExplorerTools tools)
    : ExplorerModeBase(ExplorerMode::DRAWLINE, tools) {}


ExplorerMode ExplorerModeDrawLine::HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = tools_.painter->GetCell(event->scenePos());
    if (!tools_.board->IsCell(cell)) {
        // do nothing - its not a click on the board
        return ExplorerMode::DRAWLINE;
    }
    if (cell != tools_.storage->line_point_b) {
        tools_.storage->line_point_b = cell;
        if (tools_.storage->pending_line) {
            delete tools_.storage->pending_line;
        }
        qDebug() << tools_.storage->line_point_a << "  " << tools_.storage->line_point_b << "\n";
        tools_.storage->pending_line = tools_.painter->DrawLineAB(tools_.storage->line_point_a, tools_.storage->line_point_b);
    }
    return ExplorerMode::DRAWLINE;
}


ExplorerMode ExplorerModeDrawLine::HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    tools_.storage->lines.push_back(tools_.storage->pending_line);
    tools_.storage->pending_line = nullptr;
    return ExplorerMode::DEFAULT;
}
