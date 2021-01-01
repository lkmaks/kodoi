#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QThread>
#include <QDir>

#include "MainWidget.h"


MainWidget::MainWidget(Settings *settings, QWidget *parent) :
    QWidget(parent), settings_(settings)
{
    qDebug() << QDir::currentPath() << endl;

    config_ = new Config();
    settings_->engine_cmd = "engine.exe";

    // set up main entities (alg, paint, common storage between modes)
    board_scene_ = new BoardScene();
    board_ = new AbstractBoard(config_->board_size);
    painter_ = new BoardPainter(config_, board_scene_);
    storage_ = new BoardContextStorage(config_);
    engine_wrapper_ = new EngineWrapper(settings_->engine_cmd, this);

    // set up grid of this widget
    auto lt = new BoardLayout(this, config_);
    setLayout(lt);

    // board_view widget
    board_view_ = new BoardView(config_, this);
    lt->addWidget(board_view_, BoardLayout::Board);
    board_view_->setScene(board_scene_);
    board_view_->setStyleSheet("border: 0px");

    // color bar widget
    ColorBar *color_bar_ = new ColorBar(config_, this);
    lt->addWidget(color_bar_, BoardLayout::Bar);

    // InfoWidget
    info_widget_ = new InfoWidget(config_, this);
    lt->addWidget(info_widget_, BoardLayout::InfoWidget);

    // EngineViewer manager
    engine_viewer_ = new EngineViewer(color_bar_, painter_, info_widget_, &storage_->pondering_epoch_id);

    // create tools for high-entity managers
    BoardExplorerTools tools;
    tools.config = config_;
    tools.settings = settings_;
    tools.board = board_;
    tools.painter = painter_;
    tools.engine_wrapper = engine_wrapper_;
    tools.color_bar = color_bar_;
    tools.info_widget = info_widget_;
    tools.engine_viewer = engine_viewer_;
    tools.storage = storage_;

    QObject::connect(board_scene_, &BoardScene::mousePressEventSignal, this, &MainWidget::HandleBoardSceneMousePressEvent);
    QObject::connect(board_scene_, &BoardScene::mouseReleaseEventSignal, this, &MainWidget::HandleBoardSceneMouseReleaseEvent);
    QObject::connect(board_scene_, &BoardScene::mouseMoveEventSignal, this, &MainWidget::HandleBoardSceneMouseMoveEvent);
    QObject::connect(board_scene_, &BoardScene::keyPressEventSignal, this, &MainWidget::HandleBoardSceneKeyPressEvent);

    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStarted, tools.engine_viewer, &EngineViewer::PonderingStarted);
    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStopped, tools.engine_viewer, &EngineViewer::PonderingStopped);
    QObject::connect(engine_wrapper_, &EngineWrapper::ErrorOccured, tools.engine_viewer, &EngineViewer::EngineErrorOccured);
    QObject::connect(engine_wrapper_, &EngineWrapper::NbestUpdated, tools.engine_viewer, &EngineViewer::NbestUpdated);

    QObject::connect(info_widget_, &InfoWidget::NbestValueChanged, this, &MainWidget::NbestValueChanged);

    storage_ = new BoardContextStorage(config_);

    default_mode_ = new ExplorerModeDefault(tools);
    draw_line_mode_ = new ExplorerModeDrawLine(tools);

    // set initial mode: default
    current_mode_ = default_mode_;
}


void MainWidget::AppSettingsUpdated(SettingsField field) {
    if (field == SettingsField::ENGINE_CMD) {
        engine_wrapper_->ForceStop();
        storage_->engine_state = EngineState::STOPPED;
        engine_wrapper_->SetEngineCmd(settings_->engine_cmd);
        engine_viewer_->PonderingStopped();
    }
}

// scene events

void MainWidget::HandleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMousePressEvent(event));
}

void MainWidget::HandleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMouseReleaseEvent(event));
}

void MainWidget::HandleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMouseMoveEvent(event));
}

void MainWidget::HandleBoardSceneKeyPressEvent(QKeyEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleKeyPressEvent(event));
}

// info widget events

void MainWidget::NbestValueChanged(int new_value) {
   current_mode_ = TranslateModeToPtr(current_mode_->NbestValueChanged(new_value));
}


/// utility

ExplorerModeBase *MainWidget::TranslateModeToPtr(ExplorerMode mode) {
    if (mode == ExplorerMode::DEFAULT) {
        return default_mode_;
    }
    else if (mode == ExplorerMode::DRAWLINE) {
        return draw_line_mode_;
    }
    else {
        return nullptr;
    }
}
