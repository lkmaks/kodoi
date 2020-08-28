#include "EngineViewer.h"
#include <QDebug>

EngineViewer::EngineViewer(const BoardTools &tools) : tools_(tools) {}

void EngineViewer::NbestUpdated(const EngineWrapper::NbestUpdate &upd) {
    qreal y = ValueToPortionLevel(upd.value, 10000);
    if (upd.thinking_as == StoneColor::WHITE) {
        y *= -1;
    }
    qDebug() << "set to " << y << endl;
    tools_.color_bar->SmoothSetProportionLevel(y);
    if (y > 0) {
        tools_.color_bar->SetBotWhiteText(QString::number(-abs(upd.value)));
    }
    else if (y < 0) {
        tools_.color_bar->SetTopBlackText("+" + QString::number(abs(upd.value)));
    }
    else {
        tools_.color_bar->SetBotWhiteText("0");
    }
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
