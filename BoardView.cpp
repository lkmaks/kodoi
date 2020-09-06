#include "BoardView.h"
#include <QResizeEvent>
#include <iostream>
#include "MainWidget.h"
#include <QDebug>

BoardView::BoardView(const Config *config, QWidget *parent) : QGraphicsView (parent), config_(config)
{
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}


void BoardView::resizeEvent(QResizeEvent *event) {
    //qDebug() << "resizeEvent start: " << event->size() << endl;
    //qDebug() << "at the start: " << size() << endl;
    if (event->oldSize().width() == -1) {
        qreal k = (qreal)qMin(width(), height()) /
                ((config_->board_scene_cell_size) * (config_->board_size - 1) + config_->board_scene_coordinates_padding);
        scale(k, k);
    }
    else {
        qreal prev_w = event->oldSize().width(), prev_h = event->oldSize().height();
        qreal w = event->size().width(), h = event->size().height();
        qreal k = qMin(w, h) / qMin(prev_w, prev_h);
        scale(k, k);
    }
    //qDebug() << "sender: " << this->sender() << endl;
    //qDebug() << "at the end: " << size() << endl;
}

//int BoardView::heightForWidth(int w) const {
//    return w;
//}

//bool BoardView::hasHeightForWidth() const {
//    return true;
//}
