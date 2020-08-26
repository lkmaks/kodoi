#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include "Config.h"

class MainWidget;

class BoardView : public QGraphicsView
{
private:
    const Config *config_;
public:
    BoardView(const Config *config, QWidget *widget = nullptr);
    void resizeEvent(QResizeEvent *event) override;
//    int heightForWidth(int w) const override;
//    bool hasHeightForWidth() const override;
};

#endif // BOARDVIEW_H
