#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>

class MainWidget;

class BoardScene : public QGraphicsScene
{
private:
    MainWidget *main_widget;
public:
    BoardScene(MainWidget *mwidget = nullptr);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *keyEvent);
};

#endif // BOARDSCENE_H
