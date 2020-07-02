#include "mainwidget.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    // set up grid of this widget
    QGridLayout *grid = new QGridLayout();
    setLayout(grid);

    // set up view & scene
    board_view_ = new BoardView(this, this);
    layout()->addWidget(board_view_);

    board_scene_ = new BoardScene(this);
    board_view_->setScene(board_scene_);

    board_view_->setBackgroundBrush(QBrush(QColor("#f0dcb7"), Qt::SolidPattern));

    // set up essential board objects (alg and paint)
    board_ = new AbstractBoard(config_);
    board_painter_ = new BoardPainter(config_, board_scene_);

    // set initial mode
    current_mode_ = DEFAULT;
}

void MainWidget::handleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (current_mode_ == DEFAULT) {
      default_mode_->HandleMousePressEvent(event);
  }
  else if (current_mode_ == DRAWLINE) {
      draw_line_mode_->HandleMousePressEvent(event);
  }
}

void MainWidget::handleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (current_mode_ == DEFAULT) {
        default_mode_->HandleMouseReleaseEvent(event);
    }
    else if (current_mode_ == DRAWLINE) {
        draw_line_mode_->HandleMouseReleaseEvent(event);
    }
}

void MainWidget::handleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (current_mode_ == DEFAULT) {
        default_mode_->HandleMouseMoveEvent(event);
    }
    else if (current_mode_ == DRAWLINE) {
        draw_line_mode_->HandleMouseMoveEvent(event);
    }
}

void MainWidget::handleBoardSceneKeyEvent(QKeyEvent *event) {
    if (current_mode_ == DEFAULT) {
        default_mode_->HandleKeyPressEvent(event);
    }
    else if (current_mode_ == DRAWLINE) {
        draw_line_mode_->HandleKeyPressEvent(event);
    }
}
