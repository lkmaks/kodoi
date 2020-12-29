#include "EngineViewer.h"
#include <QDebug>

EngineViewer::EngineViewer(ColorBar *color_bar, BoardPainter *painter, InfoWidget *info_widget, EngineEpochId *epoch_id) {
    color_bar_ = color_bar;
    painter_ = painter;
    info_widget_ = info_widget;
    epoch_id_ = epoch_id;
    PonderingStopped();
}


void EngineViewer::PonderingStarted() {
    info_widget_->SetEngineStateText("pondering started");
}


void EngineViewer::PonderingStopped() {
    info_widget_->SetEngineStateText("pondering stopped");
    for (QGraphicsItem *circle : eval_circles_) {
        painter_->RemoveItem(circle);
    }
    eval_circles_.clear();
    color_bar_->SmoothSetProportionLevel(0);
    color_bar_->SetTopBlackText("");
    color_bar_->SetBotWhiteText("");
}


void EngineViewer::EngineErrorOccured() {
    info_widget_->SetEngineStateText("engine error");
}

void EngineViewer::NbestUpdated(const EngineWrapper::NbestUpdate &upd) {
    // everything works in one GUI thread, so can access epoch_id
    if (upd.epoch_id != *epoch_id_) {
        // old epoch, useless nbest update
        return;
    }

    qreal y = ValueToPortionLevel(upd.value, 10000);
    if (upd.thinking_as == StoneColor::WHITE) {
        y *= -1;
    }
    //qDebug() << "set to " << y << endl;
    color_bar_->SmoothSetProportionLevel(y);
    if (y > 0) {
        color_bar_->SetTopBlackText("");
        color_bar_->SetBotWhiteText(QString::number(-abs(upd.value)));
    }
    else if (y < 0) {
        color_bar_->SetTopBlackText("+" + QString::number(abs(upd.value)));
        color_bar_->SetBotWhiteText("");
    }
    else {
        color_bar_->SetTopBlackText("");
        color_bar_->SetBotWhiteText("0");
    }

    for (QGraphicsItem *circle : eval_circles_) {
        painter_->RemoveItem(circle);
    }
    eval_circles_.clear();

    for (auto &line : upd.play_lines) {
        auto circle = painter_->DrawEvalCircle(line.line[0], ValueToEvalCircleColor(line.value, 10000));
        eval_circles_.push_back(circle);
    }

    info_widget_->SetEngineStateText("Pondering on: depth " +
                                           QString::number(upd.depth_range.first) +
                                           "-" +
                                           QString::number(upd.depth_range.second));
}

qreal EngineViewer::ValueToPortionLevel(int val, int val_max) {
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


QColor EngineViewer::ValueToEvalCircleColor(int val, int val_max) {
    qreal q = ValueToPortionLevel(val, val_max);
    qreal r = 255, g = 255;
    if (q >= 0) {
        r -= 255 * q;
    }
    else {
        g -= 255 * abs(q);
    }
    qDebug() << "COLOR: " << QColor({(int)r, (int)g, 0, 200}) << endl;
    return {(int)r, (int)g, 0, 200};
}
