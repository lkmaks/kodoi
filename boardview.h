#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QGraphicsView>
#include <QWidget>

class MainWidget;

class BoardView : public QGraphicsView
{
private:
    MainWidget *main_widget;
public:
    BoardView(MainWidget *mwidget = nullptr, QWidget *widget = nullptr);
    void resizeEvent(QResizeEvent *event);
};

#endif // BOARDVIEW_H
