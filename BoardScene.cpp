#include "BoardScene.h"
#include "MainWidget.h"

BoardScene::BoardScene(MainWidget *mwidget)
{
    main_widget = mwidget;
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->HandleBoardSceneMousePressEvent(event);
}

void BoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->HandleBoardSceneMouseMoveEvent(event);
}

void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->HandleBoardSceneMouseReleaseEvent(event);
}

void BoardScene::keyPressEvent(QKeyEvent *keyEvent) {
    main_widget->HandleBoardSceneKeyEvent(keyEvent);
}
