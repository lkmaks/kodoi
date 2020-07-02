#include "boardscene.h"
#include "mainwidget.h"

BoardScene::BoardScene(MainWidget *mwidget)
{
    main_widget = mwidget;
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->handleBoardSceneMousePressEvent(event);
}

void BoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->handleBoardSceneMouseMoveEvent(event);
}

void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->handleBoardSceneMouseReleaseEvent(event);
}

void BoardScene::keyPressEvent(QKeyEvent *keyEvent) {
    main_widget->handleBoardSceneKeyEvent(keyEvent);
}
