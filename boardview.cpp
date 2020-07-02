#include "boardview.h"
#include <QResizeEvent>
#include <iostream>
#include "mainwidget.h"

BoardView::BoardView(MainWidget *mwidget, QWidget *parent) : QGraphicsView (parent)
{
    main_widget = mwidget;
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}


void BoardView::resizeEvent(QResizeEvent *event) {
    if (event->oldSize().width() == -1) {
        qreal k = (qreal)qMin(width(), height()) / ((main_widget->config_.scene_cell_size + 4) * main_widget->config_.board_size);
        //std::cerr << height() << std::endl;
        scale(k, k);
    }
    else {
        qreal prev_w = event->oldSize().width(), prev_h = event->oldSize().height();
        qreal w = event->size().width(), h = event->size().height();
        qreal k = qMin(w, h) / qMin(prev_w, prev_h);
        scale(k, k);
    }
}
