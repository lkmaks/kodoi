#include "MainWidget.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QThread>
#include "ColorBar.h"
#include "EngineWrapper.h"


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{

    config_ = new Config();
    settings_ = new Settings();

    // set up main entities (alg, paint, common storage between modes)
    board_scene_ = new BoardScene(this);
    board_ = new AbstractBoard(config_);
    painter_ = new BoardPainter(config_, board_scene_);
    storage_ = new BoardContextStorage();
    engine_wrapper_ = new EngineWrapper(this);

    // set up grid of this widget
    auto lt = new BoardLayout();
    setLayout(lt);

    board_view_ = new BoardView(config_, this);
    lt->addWidget(board_view_, BoardLayout::Board);
    board_view_->setScene(board_scene_);
    board_view_->setStyleSheet("border: 0px");
    //lt->addWidget(new QTextEdit(this), BoardLayout::Board);

    ColorBar *color_bar_ = new ColorBar(config_, this);
    lt->addWidget(color_bar_, BoardLayout::Bar);

    // create tools for high-entity managers
    BoardTools tools;
    tools.config = config_;
    tools.settings = settings_;
    tools.board = board_;
    tools.painter = painter_;
    tools.storage = storage_;
    tools.engine_wrapper = engine_wrapper_;
    tools.color_bar = color_bar_;

    engine_viewer_ = new EngineViewer(tools);

//    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStarted, engine_viewer_, &BasicEngineViewer::PonderingStarted);
//    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStopped, engine_viewer_, &BasicEngineViewer::PonderingStopped);
    QObject::connect(engine_wrapper_, &EngineWrapper::NbestUpdated,
                     engine_viewer_, &EngineViewer::NbestUpdated);



    default_mode_ = new ExplorerModeDefault(tools);
    draw_line_mode_ = new ExplorerModeDrawLine(tools);

    // set initial mode: default
    current_mode_ = default_mode_;
}

void MainWidget::handleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event) { 
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMousePressEvent(event));
}

void MainWidget::handleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMouseReleaseEvent(event));
}

void MainWidget::handleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleMouseMoveEvent(event));
}

void MainWidget::handleBoardSceneKeyEvent(QKeyEvent *event) {
    current_mode_ = TranslateModeToPtr(current_mode_->HandleKeyPressEvent(event));
}

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
