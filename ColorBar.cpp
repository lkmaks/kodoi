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
    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_, this);

    bar_w = config_->bar_scene_bar_width;
    bar_half_h = config_->bar_scene_half_bar_height;

    top_container_ = scene_->addRect(0, 0,
                                     bar_w, bar_half_h,
                                     QPen(Qt::black, 1));
    top_container_->setZValue(1);

    bot_container_ = scene_->addRect(0, bar_half_h,
                                     bar_w, bar_half_h,
                                     QPen(Qt::white, 1));
    bot_container_->setZValue(1);

    mid_line_ = scene_->addLine(1, bar_half_h,
                                bar_w - 1, bar_half_h,
                                QPen(Qt::red, 3));
    mid_line_->setZValue(2);

    white_ = new QGraphicsRectWidget(Qt::white);
    white_->setGeometry(0, 0, bar_w, bar_half_h);
    scene_->addItem(white_);

    black_ = new QGraphicsRectWidget(Qt::black);
    black_->setGeometry(0, bar_half_h, bar_w, bar_half_h);
    scene_->addItem(black_);

    level_ = 0;
}

void ColorBar::resizeEvent(QResizeEvent *event) {
    view_->resize(event->size());
    if (event->oldSize().height() == -1) {
        //view_->fitInView(container_, Qt::IgnoreAspectRatio);
        qDebug() << view_->size() << endl;
        qDebug() << event->size() << endl;
        int new_w = event->size().width();
        int new_h = event->size().height();
        qreal k_x = (qreal)new_w / (bar_w * 1.09);
        qreal k_y = (qreal)new_h / (2 * bar_half_h * 1.005);
        view_->scale(k_x, k_y);
    }
    else {
        qreal old_h = event->oldSize().height();
        qreal new_h = event->size().height();
        qreal k = new_h / old_h;
        view_->scale(1, k);
    }
}


void ColorBar::NbestUpdated(const EngineWrapper::NbestUpdate &upd) {
    qreal y = ValueToLevelPortion(upd.value, 10000);
    int level = y * bar_half_h;
    if (upd.thinking_as == StoneColor::WHITE) {
        level *= -1;
    }
    SmoothSetLevel(level);
}


void ColorBar::SmoothSetLevel(int y) {
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

qreal ColorBar::ValueToLevelPortion(int val, int val_max) {
    // if mapping into [-1, 1] bar level, then
    // [0, 300] -> [0, 0.8]
    // [300, 600] -> [0.8, 0.9]
    // [600, val_max] -> [0.9, 1]
    // all linear, negative analogously
    // exact numbers may be adjusted

    int val1 = 300;
    int val2 = 600;

    QVector<QPair<int, qreal> > points;
    points.push_back({-val_max, -1});
    points.push_back({-val2, -0.9});
    points.push_back({-val1, -0.8});
    points.push_back({0, 0});
    points.push_back({val1, 0.8});
    points.push_back({val2, 0.9});
    points.push_back({val_max, 1});

    int i = 0;
    while (i < points.size() && points[i].first <= val) {
        ++i;
    }
    if (i == points.size()) {
        // val == val_max
        --i;
    }

    auto left = points[i - 1];
    auto right = points[i];
    qreal k = (right.second - left.second) / (right.first - left.first);
    qreal res = left.second + k * (val - left.first);

    return res;
}
