#include "ColorBar.h"
#include <QPropertyAnimation>
#include <QDebug>

/// QGraphicsRectWidget

QGraphicsRectWidget::QGraphicsRectWidget(QColor color) : color_(color) {}

void QGraphicsRectWidget::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *,
                                QWidget *)
{
    painter->fillRect(rect(), color_);
}


/// ColorBar

ColorBar::ColorBar(Config *config, QWidget *parent) :
    QWidget(parent), config_(config)
{
    setStyleSheet("background-color: #dddddd; border: 0px");

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_, this);

    bar_w = config_->bar_scene_bar_width;
    bar_half_h = config_->bar_scene_half_bar_height;

//    top_container_ = scene_->addRect(0, 0,
//                                     bar_w, bar_half_h,
//                                     QPen(Qt::black, 1));
//    top_container_->setZValue(1);

//    bot_container_ = scene_->addRect(0, bar_half_h,
//                                     bar_w, bar_half_h,
//                                     QPen(Qt::white, 1));
//    bot_container_->setZValue(1);

    //scene_->addRect(0, 0, bar_w, 2 * bar_half_h, QPen(Qt::black, 1.5));

//    mid_line_ = scene_->addLine(1, bar_half_h,
//                                bar_w - 1, bar_half_h,
//                                QPen(Qt::red, 10));
//    mid_line_->setZValue(2);
//    mid_rect_ = scene_->addRect(-3, bar_half_h - 3, bar_w + 6, 10, QPen(Qt::red, 2));
//    mid_rect_->setZValue(3);

    white_ = new QGraphicsRectWidget(Qt::white);
    white_->setGeometry(0, 0, bar_w, bar_half_h);
    scene_->addItem(white_);

    black_ = new QGraphicsRectWidget(Qt::black);
    black_->setGeometry(0, bar_half_h, bar_w, bar_half_h);
    scene_->addItem(black_);

    top_black_text_ = new QGraphicsTextItem();
    //top_black_text_->setScale(0.9);
    top_black_text_->setDefaultTextColor(Qt::black);
    top_black_text_->setZValue(3);
    //top_black_text_->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    scene_->addItem(top_black_text_);

    bot_white_text_ = new QGraphicsTextItem();
    bot_white_text_->setDefaultTextColor(Qt::white);
    bot_white_text_->setZValue(3);
    //bot_white_text_->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    scene_->addItem(bot_white_text_);

    level_ = 0;
}

void ColorBar::resizeEvent(QResizeEvent *event) {
    view_->resize(event->size());
    if (event->oldSize().height() == -1) {
        //view_->fitInView(container_, Qt::IgnoreAspectRatio);
        //qDebug() << view_->size() << endl;
        //qDebug() << event->size() << endl;
        int new_w = event->size().width();
        int new_h = event->size().height();
        qreal k_x = (qreal)new_w / (bar_w * 1.2);
        qreal k_y = (qreal)new_h / (2 * bar_half_h * 1.03);
        view_->scale(k_x, k_y);
    }
    else {
        qreal old_h = event->oldSize().height();
        qreal new_h = event->size().height();
        qreal k = new_h / old_h;
        view_->scale(1, k);
    }
}


void ColorBar::RapidSetProportionLevel(qreal p) {
    int y = p * bar_half_h;
    white_->setGeometry(0, 0, bar_w, bar_half_h - y);
    black_->setGeometry(0, bar_half_h - y, bar_w, bar_half_h + y);
}

void ColorBar::SmoothSetProportionLevel(qreal p) {
    int y = p * bar_half_h;
    QPropertyAnimation *anim = new QPropertyAnimation(white_, "geometry");
    anim->setDuration(2000);
    anim->setEndValue(QRect(0, 0, bar_w, bar_half_h - y));
    anim->setEasingCurve(QEasingCurve::InOutQuint);
    anim->start();

    anim = new QPropertyAnimation(black_, "geometry");
    anim->setDuration(2000);
    anim->setEndValue(QRect(0, bar_half_h - y, bar_w, bar_half_h + y));
    anim->setEasingCurve(QEasingCurve::InOutQuint);
    anim->start();
}

void ColorBar::SetTopBlackText(const QString &text) {
    top_black_text_->setPlainText(text);
    PutTextIntoWidth(top_black_text_, bar_w);
    int vpad = config_->bar_scene_value_vertical_padding;
    qreal lpad = (bar_w - top_black_text_->sceneBoundingRect().width()) / 2.0;
    top_black_text_->setPos(lpad, vpad);
}

void ColorBar::SetBotWhiteText(const QString &text) {
    bot_white_text_->setPlainText(text);
    PutTextIntoWidth(bot_white_text_, bar_w);
    int vpad = config_->bar_scene_value_vertical_padding;
    int lpad = (bar_w - bot_white_text_->sceneBoundingRect().width()) / 2;
    int text_height = bot_white_text_->sceneBoundingRect().height();
    bot_white_text_->setPos(lpad, 2 * bar_half_h - vpad - text_height);
}


void ColorBar::PutTextIntoWidth(QGraphicsTextItem *txt, int w) {
    qDebug() << "PutText" << endl;
    txt->setFont({"Arial", static_cast<int>(12)});
    int cur_w = txt->sceneBoundingRect().width();
    if (cur_w >= w) {
        qreal cur_pt_size = txt->font().pointSizeF();
        qreal new_pt_size = cur_pt_size * w / cur_w;
        qDebug() << cur_pt_size << " vs " << new_pt_size << endl;
        txt->setFont({"Arial", static_cast<int>(new_pt_size)});
        qDebug() << txt->font().pointSizeF() << endl;
    }
}
