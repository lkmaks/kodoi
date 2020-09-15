#include "EngineViewer.h"
#include <QDebug>

EngineViewer::EngineViewer(const BoardTools &tools) : tools_(tools) {
    PonderingStopped();
}


void EngineViewer::PonderingStarted() {
    tools_.info_widget->SetEngineStateText("pondering started");
}


void EngineViewer::PonderingStopped() {
    tools_.info_widget->SetEngineStateText("pondering stopped");
    for (QGraphicsItem *circle : tools_.storage->eval_circles) {
        tools_.painter->RemoveItem(circle);
    }
    tools_.storage->eval_circles.clear();
    tools_.color_bar->SmoothSetProportionLevel(0);
    tools_.color_bar->SetTopBlackText("");
    tools_.color_bar->SetBotWhiteText("");
}


void EngineViewer::EngineErrorOccured() {
    tools_.info_widget->SetEngineStateText("engine error");
}

void EngineViewer::NbestUpdated(const EngineWrapper::NbestUpdate &upd) {
    if (upd.epoch_id != tools_.storage->pondering_epoch_id) {
        // old epoch, useless nbest update
        return;
    }

    qreal y = ValueToPortionLevel(upd.value, 10000);
    if (upd.thinking_as == StoneColor::WHITE) {
        y *= -1;
    }
    //qDebug() << "set to " << y << endl;
    tools_.color_bar->SmoothSetProportionLevel(y);
    if (y > 0) {
        tools_.color_bar->SetTopBlackText("");
        tools_.color_bar->SetBotWhiteText(QString::number(-abs(upd.value)));
    }
    else if (y < 0) {
        tools_.color_bar->SetTopBlackText("+" + QString::number(abs(upd.value)));
        tools_.color_bar->SetBotWhiteText("");
    }
    else {
        tools_.color_bar->SetTopBlackText("");
        tools_.color_bar->SetBotWhiteText("0");
    }

    for (QGraphicsItem *circle : tools_.storage->eval_circles) {
        tools_.painter->RemoveItem(circle);
    }
    tools_.storage->eval_circles.clear();

    for (auto &line : upd.play_lines) {
        auto circle = tools_.painter->DrawEvalCircle(line.line[0], ValueToEvalCircleColor(line.value, 10000));
        tools_.storage->eval_circles.push_back(circle);
    }

    tools_.info_widget->SetEngineStateText("Pondering on: depth " +
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
