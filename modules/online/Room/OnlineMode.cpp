#include "OnlineMode.h"
#include <QDebug>
#include <QEventLoop>


OnlineModeBase::OnlineModeBase(OnlineMode mode, BoardOnlineTools tools) :
    mode_(mode), tools_(tools) {
}


OnlineMode OnlineModeBase::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    return mode_;
}

OnlineMode OnlineModeBase::HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    return mode_;
}

OnlineMode OnlineModeBase::HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    return mode_;
}

OnlineMode OnlineModeBase::HandleKeyPressEvent(QKeyEvent *event) {
    (void)event;
    return mode_;
}

OnlineMode OnlineModeBase::NbestValueChanged(int new_value) {
    if (tools_.storage->engine_state == EngineState::ACTIVE) {
        tools_.storage->nbest_value = new_value;
        UpdatePonderingPosition(new_value);
    }
    return mode_;
}


OnlineMode OnlineModeBase::HandleOnlineReceivedStatus(bool status) {
    (void)status;
    return mode_;
}


OnlineMode OnlineModeBase::HandleOnlineReceivedInit(BoardAction action) {
    tools_.online_board->ApplyAction(action);
    RenderVisualBoardAction(action);
    return mode_;
}


OnlineMode OnlineModeBase::HandleOnlineReceivedUpdate(BoardAction action) {
    tools_.online_board->ApplyAction(action);
    RenderVisualBoardAction(action);
    return mode_;
}


void OnlineModeBase::TryMakeMove(QPair<int, int> cell) {
    tools_.session->MakeMove(cell, tools_.online_board->GetEpochId());
}

void OnlineModeBase::TryUndo() {
    tools_.session->Undo(tools_.online_board->GetEpochId());
}

void OnlineModeBase::TryUndoUntil(QPair<int, int> cell) {
    tools_.session->UndoUntil(cell, tools_.online_board->GetEpochId());
}

void OnlineModeBase::TryRedo() {
    tools_.session->Redo(tools_.online_board->GetEpochId());
}


void OnlineModeBase::RenderMakeMove(QPair<int, int> cell) {
    // draw new stone
    auto stone_txt_pair = tools_.painter->DrawNumberedStone(cell,
                                                      opposite_color(tools_.board->GetCurrentColor()),
                                                      tools_.board->MovesCount());
    tools_.storage->stones_pos.push_back(stone_txt_pair.first);
    tools_.storage->numbers_pos.push_back(stone_txt_pair.second);

    RenderAuxiliary();
    UpdatePonderingPosition(tools_.storage->nbest_value);
}

void OnlineModeBase::RenderUndo() {
    tools_.painter->RemoveItem(tools_.storage->stones_pos.back());
    tools_.storage->stones_pos.pop_back();

    tools_.painter->RemoveItem(tools_.storage->numbers_pos.back());
    tools_.storage->numbers_pos.pop_back();

    RenderAuxiliary();
    UpdatePonderingPosition(tools_.storage->nbest_value);
}

void OnlineModeBase::RenderUndoUntil() {
    while (tools_.storage->stones_pos.size() > tools_.board->MovesCount()) {
        tools_.painter->RemoveItem(tools_.storage->stones_pos.back());
        tools_.storage->stones_pos.pop_back();

        tools_.painter->RemoveItem(tools_.storage->numbers_pos.back());
        tools_.storage->numbers_pos.pop_back();
    }
    RenderAuxiliary();
    UpdatePonderingPosition(tools_.storage->nbest_value);
}

void OnlineModeBase::RenderRedo() {
    auto cell = tools_.board->GetLastMove();
    auto stone_txt_pair = tools_.painter->DrawNumberedStone(cell,
                                                      opposite_color(tools_.board->GetCurrentColor()),
                                                      tools_.board->MovesCount());
    tools_.storage->stones_pos.push_back(stone_txt_pair.first);
    tools_.storage->numbers_pos.push_back(stone_txt_pair.second);

    RenderAuxiliary();
    UpdatePonderingPosition(tools_.storage->nbest_value);
}


void OnlineModeBase::RenderMarks() {
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

void OnlineModeBase::RenderVisualBoardAction(BoardAction action) {
    if (action.type == BoardActionType::MOVE) {
        RenderMakeMove(action.coords);
    }
    else if (action.type == BoardActionType::UNDO) {
        RenderUndo();
    }
    else if (action.type == BoardActionType::UNDO_UNTIL) {
        RenderUndoUntil();
    }
    else if (action.type == BoardActionType::REDO) {
        RenderRedo();
    }
}

void OnlineModeBase::RenderLastMoveHighlight() {
    if (tools_.storage->last_stone_border_highlight != nullptr) {
        tools_.painter->RemoveItem(tools_.storage->last_stone_border_highlight);
        tools_.storage->last_stone_border_highlight = nullptr;
    }
    if (!tools_.board->Empty() && tools_.storage->view_last_move_highlight) {
        tools_.storage->last_stone_border_highlight = tools_.painter->HighlightStoneBorder(tools_.board->GetLastMove(), Qt::red);
    }
}

void OnlineModeBase::RenderAuxiliary() {
    RenderMarks();
    RenderLastMoveHighlight();
}

void OnlineModeBase::SetViewMarks(bool show) {
    tools_.storage->view_marks = show;
    RenderMarks();
}

void OnlineModeBase::SetLastMoveHighlight(bool show) {
    tools_.storage->view_last_move_highlight = show;
    RenderLastMoveHighlight();
}

void OnlineModeBase::StartPondering() {
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

void OnlineModeBase::UpdatePonderingPosition(int nbest_value) {
    if (tools_.storage->engine_state == EngineState::ACTIVE) {
        tools_.engine_wrapper->StopThinking();
        tools_.engine_wrapper->StartThinking(tools_.board->GetEngineFormatPosition(), nbest_value);
        ++tools_.storage->pondering_epoch_id;
    }
}

void OnlineModeBase::StopPondering() {
    tools_.storage->engine_state = EngineState::STOPPING;
    tools_.engine_wrapper->StopThinking();
    tools_.engine_wrapper->ForceStop();
    tools_.storage->engine_state = EngineState::STOPPED;
}

//void OnlineModeBase::SetViewMarks(bool view) {
//    settings_->view_marks = view;
//    if (view) {
//        for (auto mark : tools_.storage->marks) {
//            // TODO: unhide
//        }
//    }
//}

OnlineModeDefault::OnlineModeDefault(BoardOnlineTools tools)
    : OnlineModeBase(OnlineMode::DEFAULT, tools) {}

OnlineMode OnlineModeDefault::HandleMousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = tools_.painter->GetCell(event->scenePos());
    if (!tools_.board->IsCell(cell)) {
        // do nothing - its not a click on the board
        return OnlineMode::DEFAULT;
    }
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            tools_.storage->line_point_a = cell;
            tools_.storage->line_point_b = cell;
            return OnlineMode::DRAWLINE;
        }
        else {
            if (tools_.board->GetCell(cell) == CellType::EMPTY) {
                TryMakeMove(cell);
            }
            else {
                TryUndoUntil(cell);
            }
        }
    }
    else if (event->button() == Qt::RightButton) {
        TryUndo();
    }
    return OnlineMode::DEFAULT;
}


OnlineMode OnlineModeDefault::HandleKeyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_C) {
        // clear temporary objects
        for (auto line : tools_.storage->lines) {
            delete line;
        }
        tools_.storage->lines.clear();
    }
    else if (event->key() == Qt::Key_Left) {
        TryUndo();
    }
    else if (event->key() == Qt::Key_Right) {
        TryRedo();
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
    return OnlineMode::DEFAULT;
}


/// DRAWLINE


OnlineModeDrawLine::OnlineModeDrawLine(BoardOnlineTools tools)
    : OnlineModeBase(OnlineMode::DRAWLINE, tools) {}


OnlineMode OnlineModeDrawLine::HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto cell = tools_.painter->GetCell(event->scenePos());
    if (!tools_.board->IsCell(cell)) {
        // do nothing - its not a click on the board
        return OnlineMode::DRAWLINE;
    }
    if (cell != tools_.storage->line_point_b) {
        tools_.storage->line_point_b = cell;
        if (tools_.storage->pending_line) {
            delete tools_.storage->pending_line;
        }
        qDebug() << tools_.storage->line_point_a << "  " << tools_.storage->line_point_b << "\n";
        tools_.storage->pending_line = tools_.painter->DrawLineAB(tools_.storage->line_point_a, tools_.storage->line_point_b);
    }
    return OnlineMode::DRAWLINE;
}


OnlineMode OnlineModeDrawLine::HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    (void)event;
    tools_.storage->lines.push_back(tools_.storage->pending_line);
    tools_.storage->pending_line = nullptr;
    return OnlineMode::DEFAULT;
}
