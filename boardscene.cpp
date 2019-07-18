#include "boardscene.h"
#include "mainwidget.h"

BoardScene::BoardScene(MainWidget *mwidget)
{
    main_widget = mwidget;
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    main_widget->handleBoardSceneMouseEvent(event);
}

void BoardScene::keyPressEvent(QKeyEvent *keyEvent) {
    main_widget->handleBoardSceneKeyEvent(keyEvent);
}
