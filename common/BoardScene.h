#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>


class BoardScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BoardScene(QObject *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *keyEvent);

signals:
    void mousePressEventSignal(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEventSignal(QGraphicsSceneMouseEvent *event);
    void mouseMoveEventSignal(QGraphicsSceneMouseEvent *event);
    void keyPressEventSignal(QKeyEvent *keyEvent);
};

#endif // BOARDSCENE_H
