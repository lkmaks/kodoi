#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QThread>
#include <QDir>
#include <QEventLoop>
#include <QVBoxLayout>

#include "MainWindow.h"
#include "OnlineWidget.h"


OnlineWidget::OnlineWidget(Config *config, Settings *settings, OnlineSession *sess, RoomId room_id, QWidget *parent) :
    QWidget(parent), config_(config), settings_(settings), room_id_(room_id), session_(sess)
{
    // set up Online entities (alg, paint, common storage between modes)
    board_ = new AbstractBoard(config_->board_size);
    online_board_ = new OnlineBoard(board_);
    board_scene_ = new BoardScene();
    painter_ = new BoardPainter(config_, board_scene_);
    storage_ = new OnlineContextStorage(config_);
    engine_wrapper_ = new EngineWrapper(settings_->engine_cmd, this);

    // set up grid of this widget
    auto blt = new BoardLayout(this, config_);
    setLayout(blt);

    // leave widget
    leave_widget_ = new LeaveWidget(this);
    blt->addWidget(leave_widget_, BoardLayout::LeaveWidget);

    // board_view widget
    board_view_ = new BoardView(config_, this);
    blt->addWidget(board_view_, BoardLayout::Board);
    board_view_->setScene(board_scene_);
    board_view_->setStyleSheet("border: 0px");

    // color bar widget
    ColorBar *color_bar_ = new ColorBar(config_, this);
    blt->addWidget(color_bar_, BoardLayout::Bar);

    // InfoWidget
    info_widget_ = new InfoWidget(config_, this);
    blt->addWidget(info_widget_, BoardLayout::InfoWidget);


    // EngineViewer manager
    engine_viewer_ = new EngineViewer(color_bar_, painter_, info_widget_, &storage_->pondering_epoch_id);

    // current session
    session_ = sess;

    // create tools for high-entity managers
    BoardOnlineTools tools;
    tools.config = config_;
    tools.settings = settings_;
    tools.board = board_;
    tools.online_board = online_board_;
    tools.painter = painter_;
    tools.engine_wrapper = engine_wrapper_;
    tools.color_bar = color_bar_;
    tools.info_widget = info_widget_;
    tools.engine_viewer = engine_viewer_;
    tools.storage = storage_;
    tools.session = session_;

    QObject::connect(leave_widget_, &LeaveWidget::Leave, this, &OnlineWidget::Leave);

    QObject::connect(board_scene_, &BoardScene::mousePressEventSignal, this, &OnlineWidget::HandleBoardSceneMousePressEvent);
    QObject::connect(board_scene_, &BoardScene::mouseReleaseEventSignal, this, &OnlineWidget::HandleBoardSceneMouseReleaseEvent);
    QObject::connect(board_scene_, &BoardScene::mouseMoveEventSignal, this, &OnlineWidget::HandleBoardSceneMouseMoveEvent);
    QObject::connect(board_scene_, &BoardScene::keyPressEventSignal, this, &OnlineWidget::HandleBoardSceneKeyPressEvent);

    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStarted, tools.engine_viewer, &EngineViewer::PonderingStarted);
    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStopped, tools.engine_viewer, &EngineViewer::PonderingStopped);
    QObject::connect(engine_wrapper_, &EngineWrapper::ErrorOccured, tools.engine_viewer, &EngineViewer::EngineErrorOccured);
    QObject::connect(engine_wrapper_, &EngineWrapper::NbestUpdated, tools.engine_viewer, &EngineViewer::NbestUpdated);

    QObject::connect(info_widget_, &InfoWidget::NbestValueChanged, this, &OnlineWidget::NbestValueChanged);

    //QObject::connect(client_, &OnlineClient::ReceivedStatus, this, &OnlineWidget::HandleOnlineReceivedStatus);
    QObject::connect(session_, &OnlineSession::ReceivedInit, this, &OnlineWidget::HandleOnlineReceivedInit);
    QObject::connect(session_, &OnlineSession::ReceivedUpdate, this, &OnlineWidget::HandleOnlineReceivedUpdate);

    storage_ = new OnlineContextStorage(config_);

    default_mode_ = new OnlineModeDefault(tools);
    draw_line_mode_ = new OnlineModeDrawLine(tools);

    // set initial mode: default
    current_mode_ = default_mode_;

    session_->NeedInit(); // we are ready to receive all init

    /// testing
//    info_widget_->SetEngineStateText(QDir::currentPath());
//    client_->Create(1);
//    client_->Enter(1);
//    client_->SetRoomId(1);
//    QEventLoop loop;
//    QObject::connect(client_, &OnlineClient::ReceivedStatus, &loop, &QEventLoop::quit);
//    loop.exec();
}

void OnlineWidget::AppSettingsUpdated(SettingsField field) {
    if (field == SettingsField::ENGINE_CMD) {
        engine_wrapper_->ForceStop();
        storage_->engine_state = EngineState::STOPPED;
        engine_wrapper_->SetEngineCmd(settings_->engine_cmd);
        engine_viewer_->PonderingStopped();
    }
}


// scene events

void OnlineWidget::HandleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMousePressEvent(event));
}

void OnlineWidget::HandleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMouseReleaseEvent(event));
}

void OnlineWidget::HandleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMouseMoveEvent(event));
}

void OnlineWidget::HandleBoardSceneKeyPressEvent(QKeyEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleKeyPressEvent(event));
}

// info widget events

void OnlineWidget::NbestValueChanged(int new_value) {
   current_mode_ = TranslateModeToPtr(current_mode_->NbestValueChanged(new_value));
}


// online client events

void OnlineWidget::HandleOnlineReceivedStatus(bool status) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleOnlineReceivedStatus(status));
}

void OnlineWidget::HandleOnlineReceivedInit(BoardAction action) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleOnlineReceivedInit(action));
}

void OnlineWidget::HandleOnlineReceivedUpdate(BoardAction action) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleOnlineReceivedUpdate(action));
}



// Leave

void OnlineWidget::Leave() {
    session_->LeaveRoom(room_id_);
    bool res = false;
    QEventLoop loop;
    QObject::connect(session_, &OnlineSession::ReceivedStatus, this, [&loop, &res](QString status){
        res = (status == "ok");
        loop.quit();
    });
    loop.exec();
    if (res) {
        static_cast<MainWindow*>(parent())->LeaveRoom();
    }
}


/// utility

OnlineModeBase *OnlineWidget::TranslateModeToPtr(OnlineMode mode) {
    if (mode == OnlineMode::DEFAULT) {
        return default_mode_;
    }
    else if (mode == OnlineMode::DRAWLINE) {
        return draw_line_mode_;
    }
    else {
        return nullptr;
    }
}
