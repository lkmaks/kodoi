#include "MainWidget.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QThread>
#include <QEventLoop>
#include <QBoxLayout>

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
    QBoxLayout *box = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
    setLayout(box);

    // set up view widgets
    board_view_ = new BoardView(config_, this);
    layout()->addWidget(board_view_);
    board_view_->setScene(board_scene_);

    engine_viewer_ = new BasicEngineViewer(this);
    layout()->addWidget(engine_viewer_);

    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStarted, engine_viewer_, &BasicEngineViewer::PonderingStarted);
    QObject::connect(engine_wrapper_, &EngineWrapper::EngineStopped, engine_viewer_, &BasicEngineViewer::PonderingStopped);
    QObject::connect(engine_wrapper_, &EngineWrapper::NbestUpdated, engine_viewer_, &BasicEngineViewer::NbestUpdated);

    // set up mode objects and with tools for them
    ExplorerModeTools tools;
    tools.config = config_;
    tools.settings = settings_;
    tools.board = board_;
    tools.painter = painter_;
    tools.storage = storage_;
    tools.engine_wrapper = engine_wrapper_;

    default_mode_ = new ExplorerModeDefault(tools);
    draw_line_mode_ = new ExplorerModeDrawLine(tools);

    // set initial mode: default
    current_mode_ = default_mode_;

    // example:
//    EngineWrapper *wrapper = new EngineWrapper();
//    QEventLoop loop;
//    QObject::connect(wrapper, &EngineWrapper::EngineStarted, &loop, &QEventLoop::quit);
//    wrapper->Start();
//    loop.exec();
//    wrapper->Setup({});
//    EngineWrapper::Position pos;
//    pos.board_width = pos.board_height = 15;
//    pos.seq = {{7,7}, {6,7}, {5, 9}};
//    wrapper->StartThinking(pos, 3);
//    loop.connect(wrapper, &EngineWrapper::NbestUpdated, [](const EngineWrapper::NbestUpdate &upd) {
//        qDebug() << upd.value << endl;
//    });
//    loop.exec();
//    qDebug() << "ALALALALAALALA" << endl;
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
    if (mode == DEFAULT) {
        return default_mode_;
    }
    else if (mode == DRAWLINE) {
        return draw_line_mode_;
    }
    else {
        return nullptr;
    }
}
