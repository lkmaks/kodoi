#include "mainwidget.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout();
    setLayout(grid);

    init_board_graphics();
    init_board_alg();

    current_mode = "default";
}


void MainWidget::init_board_alg() {
    board_arr.resize(board_size);
    for (int i = 0; i < board_size; ++i) {
        board_arr[i].resize(board_size);
    }
}


void MainWidget::init_board_graphics() {
    board_view = new BoardView(this, this);
    layout()->addWidget(board_view);

    board_scene = new BoardScene(this);
    board_view->setScene(board_scene);

    QPen line_pen(Qt::black);
    line_pen.setCosmetic(true);
    line_pen.setWidth(2);
    for (int i = 0; i < board_size; ++i) {
        board_scene->addLine(0, i * cell_size, (board_size - 1) * cell_size, i * cell_size, line_pen);
        board_scene->addLine(i * cell_size, 0, i * cell_size, (board_size - 1) * cell_size, line_pen);
    }

    QBrush dot_brush(Qt::black);
    qreal r = (qreal)cell_size / 12;
    board_scene->addEllipse(cell_size * 7 - r, cell_size * 7 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene->addEllipse(cell_size * 3 - r, cell_size * 3 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene->addEllipse(cell_size * 3 - r, cell_size * 11 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene->addEllipse(cell_size * 11 - r, cell_size * 3 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene->addEllipse(cell_size * 11 - r, cell_size * 11 - r, 2 * r, 2 * r, line_pen, dot_brush);

    for (int i = 0; i < board_size; ++i) {
        QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
        txt->setPos(i * cell_size - cell_size / 4, cell_size * (board_size - 1));
        txt->setText(QString((char)('A' + i)));
        board_scene->addItem(txt);
    }

    for (int i = 0; i < board_size; ++i) {
        QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
        txt->setPos(-cell_size, cell_size * (board_size - i - 1) - cell_size / 2);
        QString text = QString::number(i + 1);
        if (i < 9) {
            text = " " + text;
        }
        txt->setText(text);
        board_scene->addItem(txt);
    }
}


void MainWidget::putStone(int i, int j, int color, int number) {
    QPen line_pen(Qt::black);
    line_pen.setCosmetic(true);
    QBrush stone_brush;
    QBrush number_brush;
    stone_brush.setStyle(Qt::SolidPattern);
    number_brush.setStyle(Qt::SolidPattern);
    if (color == 1) {
        stone_brush.setColor(Qt::black);
        number_brush.setColor(Qt::white);
    }
    else {
        stone_brush.setColor(Qt::white);
        number_brush.setColor(Qt::black);
    }
    qreal stone_width = cell_size - (qreal)cell_size / stone_gap_factor;
    QGraphicsItem *stone = board_scene->addEllipse(cell_size * i - stone_width / 2, cell_size * j - stone_width / 2, stone_width, stone_width, line_pen, stone_brush);
    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    txt->setText(QString::number(number));
    QRectF bR = txt->sceneBoundingRect();
    txt->setPos(cell_size * i - bR.width() / 2, cell_size * j - bR.height() / 2);
    txt->setBrush(number_brush);
    board_scene->addItem(txt);

    stones.push_back(stone);
    numbers.push_back(txt);
}


void MainWidget::drawLineAB() {
    QPoint from(line_point_a.first * cell_size, line_point_a.second * cell_size);
    QPoint to(line_point_b.first * cell_size, line_point_b.second * cell_size);
    QPen line_pen(Qt::red);
    line_pen.setWidth(3);
    auto line = board_scene->addLine(from.x(), from.y(), to.x(), to.y(), line_pen);
    lines.push_back(line);
}


void MainWidget::addStone(int i, int j) {
    position.push_back({i, j});
    int color = 1;
    if (position.size() % 2 == 0) {
        color = 2;
    }
    board_arr[i][j] = color;
    putStone(i, j, color, position.size());
}


void MainWidget::undo_until(int i, int j) {
    while (!position.empty() && position.back() != QPair<int, int>(i, j)) {
        undo();
    }
}


void MainWidget::undo() {
    if (!position.empty()) {
        int i = position.back().first, j = position.back().second;
        board_arr[i][j] = 0;
//        board_scene->removeItem(stones.back());
//        board_scene->removeItem(numbers.back());
//        delete stones.back();
//        delete numbers.back();
        stones.back()->hide();
        numbers.back()->hide();
        position.pop_back();
        stones.pop_back();
        numbers.pop_back();
    }
}


void MainWidget::handleRightClickOn(int i, int j) {
    if (current_mode == "default") {
        undo();
    }
}

void MainWidget::handleLeftClickOn(int i, int j) {
    if (current_mode == "default") {
        if (board_arr[i][j] == 0) {
            addStone(i, j);
        }
        else {
            undo_until(i, j);
        }
    }
    else if (current_mode == "draw_line") {
        if (step == 0) {
            line_point_a = qMakePair(i, j);
            step = 1;
        }
        else if (step == 1) {
            line_point_b = qMakePair(i, j);
            step = 0;
            drawLineAB();
        }
    }
}


void MainWidget::handleBoardSceneMouseEvent(QGraphicsSceneMouseEvent *event) {
    auto button = event->button();
    auto point = event->scenePos();
    int x = point.x(), y = point.y();

    int i = (x + cell_size / 2) / cell_size;
    int j = (y + cell_size / 2) / cell_size;

    if (0 <= i && i < board_size && 0 <= j && j < board_size) {
        if (button == Qt::LeftButton) {
            handleLeftClickOn(i, j);
        }
        else if (button == Qt::RightButton) {
            handleRightClickOn(i, j);
        }
    }
}

void MainWidget::handleBoardSceneKeyEvent(QKeyEvent *event) {
    auto key = event->key();
    if (current_mode == "default") {
        if (key == Qt::Key_A) {
            current_mode = "draw_line";
        }
    }
    else if (current_mode == "draw_line") {
        if (key == Qt::Key_Escape || key == Qt::Key_Q) {
            current_mode = "default";
            for (auto line : lines) {
                board_scene->removeItem(line);
                delete line;
            }
            lines.clear();
        }
    }
}
