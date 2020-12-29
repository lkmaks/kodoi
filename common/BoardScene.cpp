#include "BoardScene.h"

BoardScene::BoardScene(QObject *parent) : QGraphicsScene(parent) {}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit mousePressEventSignal(event);
}

void BoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    emit mouseMoveEventSignal(event);
}

void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    emit mouseReleaseEventSignal(event);
}

void BoardScene::keyPressEvent(QKeyEvent *event) {
    emit keyPressEventSignal(event);
}
