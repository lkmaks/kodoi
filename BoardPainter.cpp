#include "BoardPainter.h"

BoardPainter::BoardPainter(const Config *config, BoardScene *board_scene) :
    config_(config), board_scene_(board_scene), cell_size_(config->scene_cell_size),
    board_size_(config->board_size) {

    PaintEmptyBoard();
}

QPair<int, int> BoardPainter::GetCell(QPointF pos) {
    int x = pos.x(), y = pos.y();

    int i = (x + cell_size_ / 2) / cell_size_;
    int j = (y + cell_size_ / 2) / cell_size_;

    return QPair<int, int>(i, j);
}

void BoardPainter::PaintEmptyBoard() {
    // draw background
    int background_bonus_left = 50;
    int background_bonus_right = 30;
    int background_bonus_bot = 40;
    int background_bonus_top = 30;

    int background_bonus_vert = background_bonus_bot + background_bonus_top;
    int background_bonus_hor = background_bonus_left + background_bonus_right;

    int background_size_hor = cell_size_ * (board_size_ - 1) + background_bonus_hor;
    int background_size_vert = cell_size_ * (board_size_ - 1) + background_bonus_vert;

    QImage img("/home/max/qt_projects/kodoi/lightwoodbackground.jpg");
    img = img.copy(QRect(0, 0, background_size_hor, background_size_vert));

    QGraphicsItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    background->setZValue(-1000);
    background->setPos(-background_bonus_left, -background_bonus_top);
    board_scene_->addItem(background);


    // draw frame
    QPen background_frame_pen(Qt::darkGreen);
    background_frame_pen.setCosmetic(true);
    background_frame_pen.setWidth(4);

    board_scene_->addLine(-background_bonus_left,
                          -background_bonus_top,
                          background_size_hor - background_bonus_left,
                          -background_bonus_top,
                          background_frame_pen);

    board_scene_->addLine(-background_bonus_left,
                          -background_bonus_top,
                          -background_bonus_left,
                          background_size_vert - background_bonus_top,
                          background_frame_pen);

    board_scene_->addLine(-background_bonus_left,
                          background_size_vert - background_bonus_top,
                          background_size_hor - background_bonus_left,
                          background_size_vert - background_bonus_top,
                          background_frame_pen);

    board_scene_->addLine(background_size_hor - background_bonus_left,
                          -background_bonus_top,
                          background_size_hor - background_bonus_left,
                          background_size_vert - background_bonus_top,
                          background_frame_pen);

    QPen line_pen(Qt::black);
    line_pen.setCosmetic(true);
    line_pen.setWidth(2);


    for (int i = 0; i < board_size_; ++i) {
        board_scene_->addLine(0, i * cell_size_, (board_size_ - 1) * cell_size_, i * cell_size_, line_pen);
        board_scene_->addLine(i * cell_size_, 0, i * cell_size_, (board_size_ - 1) * cell_size_, line_pen);
    }

    QBrush dot_brush(Qt::black);
    qreal r = (qreal)cell_size_ / 12;
    board_scene_->addEllipse(cell_size_ * 7 - r, cell_size_ * 7 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene_->addEllipse(cell_size_ * 3 - r, cell_size_ * 3 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene_->addEllipse(cell_size_ * 3 - r, cell_size_ * 11 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene_->addEllipse(cell_size_ * 11 - r, cell_size_ * 3 - r, 2 * r, 2 * r, line_pen, dot_brush);
    board_scene_->addEllipse(cell_size_ * 11 - r, cell_size_ * 11 - r, 2 * r, 2 * r, line_pen, dot_brush);

    for (int i = 0; i < board_size_; ++i) {
        QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
        txt->setPos(i * cell_size_ - (qreal)cell_size_ / 4, (qreal)cell_size_ * (board_size_ - 1));
        txt->setText(QString((char)('A' + i)));
        board_scene_->addItem(txt);
    }

    for (int i = 0; i < board_size_; ++i) {
        QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
        txt->setPos(-cell_size_, cell_size_ * (board_size_ - i - 1) - (qreal)cell_size_ / 2);
        QString text = QString::number(i + 1);
        if (i < 9) {
            text = " " + text;
        }
        txt->setText(text);
        board_scene_->addItem(txt);
    }
}

QPair<QGraphicsItem*, QGraphicsItem*> BoardPainter::DrawNumberedStone(QPair<int, int> cell, StoneColor color, int number) {
    int i = cell.first;
    int j = cell.second;

    QPen line_pen(Qt::black);
    line_pen.setCosmetic(true);
    QBrush stone_brush;
    QBrush number_brush;
    stone_brush.setStyle(Qt::SolidPattern);
    number_brush.setStyle(Qt::SolidPattern);
    if (color == StoneColor::BLACK) {
        stone_brush.setColor(Qt::black);
        number_brush.setColor(Qt::white);
    }
    else {
        stone_brush.setColor(Qt::white);
        number_brush.setColor(Qt::black);
    }
    qreal stone_width = cell_size_ - (qreal)cell_size_ * stone_gap_factor_;
    //QGraphicsItem *stone = board_scene->addEllipse(cell_size * i - stone_width / 2, cell_size * j - stone_width / 2, stone_width, stone_width, line_pen, stone_brush);

    QImage img;
    if (color == StoneColor::BLACK) {
        img = QImage("/home/max/qt_projects/kodoi/b_succ.png");
    }
    else {
        img = QImage("/home/max/qt_projects/kodoi/w_succ.png");
    }
    QGraphicsItem *stone = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    stone->setScale((qreal)stone_width / (img.width()) * 1.05);
    stone->setPos(cell_size_ * i - stone_width / 2 + 0.5, cell_size_ * j - stone_width / 2 + 0.5);
    board_scene_->addItem(stone);

    QGraphicsSimpleTextItem *txt = nullptr;
    if (number != -1) {
        txt = new QGraphicsSimpleTextItem();
        txt->setText(QString::number(number));
        QRectF bR = txt->sceneBoundingRect();
        txt->setPos(cell_size_ * i - bR.width() / 2, cell_size_ * j - bR.height() / 2);
        txt->setBrush(number_brush);
        board_scene_->addItem(txt);
    }

    return QPair<QGraphicsItem*, QGraphicsItem*>(stone, txt);
}


QGraphicsItem *BoardPainter::DrawLineAB(QPair<int, int> line_point_a, QPair<int, int> line_point_b) {
    QPoint from(line_point_a.first * cell_size_, line_point_a.second * cell_size_);
    QPoint to(line_point_b.first * cell_size_, line_point_b.second * cell_size_);
    QPen line_pen(QColor("#99ee0000"));
    line_pen.setWidth(8);
    auto line = board_scene_->addLine(from.x(), from.y(), to.x(), to.y(), line_pen);
    line->setZValue(-1);
    return line;
}

QGraphicsItem *BoardPainter::DrawMoveMark(QPair<int, int> cell, StoneColor color) {
    int i = cell.first;
    int j = cell.second;

    qreal mark_width = cell_size_ / 3;
    QImage img;
    if (color == StoneColor::BLACK) {
        img = QImage("/home/max/qt_projects/kodoi/b_succ.png");
    }
    else {
        img = QImage("/home/max/qt_projects/kodoi/w_succ.png");
    }

    QGraphicsItem *mark = new QGraphicsPixmapItem(QPixmap::fromImage(img));

    mark->setScale((qreal)mark_width / (img.width()));
    mark->setPos(cell_size_ * i - mark_width / 2 + 0.3, cell_size_ * j - mark_width / 2 + 0.3);

    board_scene_->addItem(mark);

    return mark;
}
