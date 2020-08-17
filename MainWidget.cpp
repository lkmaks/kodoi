#include "MainWidget.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QThread>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    config_ = new Config();
    settings_ = new Settings();

    // set up grid of this widget
    QGridLayout *grid = new QGridLayout();
    setLayout(grid);

    // set up view & scene
    board_view_ = new BoardView(config_, this);
    layout()->addWidget(board_view_);

    board_scene_ = new BoardScene(this);
    board_view_->setScene(board_scene_);

    //board_view_->setBackgroundBrush(QBrush(QColor("#f0dcb7"), Qt::SolidPattern));

    // set up main entities (alg, paint, common storage between modes)
    board_ = new AbstractBoard(config_);
    painter_ = new BoardPainter(config_, board_scene_);
    storage_ = new CommonModeDataStorage();

    // set up mode objects
    ExplorerModeTools tools;
    tools.config = config_;
    tools.settings = settings_;
    tools.board = board_;
    tools.painter = painter_;
    tools.storage = storage_;

    default_mode_ = new ExplorerModeDefault(tools);
    draw_line_mode_ = new ExplorerModeDrawLine(tools);

    // set initial mode: default
    current_mode_ = default_mode_;

    EngineWrapper *wrapper = new EngineWrapper();
//    QThread::sleep(3);
//    qDebug() << "after a while: " << wrapper->brain_proc_.processId() << "\n";
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
