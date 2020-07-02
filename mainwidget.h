#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QSet>
#include "abstractboard.h"
#include "boardscene.h"
#include "boardview.h"
#include "explorermode.h"
#include "boardpainter.h"
#include "commonmodedatastorage.h"
#include "config.h"

class MainWidget : public QWidget
{
    Q_OBJECT

    friend BoardView;

private:
    BoardScene *board_scene_;
    BoardView *board_view_;

    AbstractBoard *board_;
    BoardPainter *board_painter_;
    CommonModeDataStorage *storage_;

    ExplorerMode current_mode_;
    ExplorerModeDefault *default_mode_;
    ExplorerModeDrawLine *draw_line_mode_;

    Config config_;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    void handleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event);
    void handleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *  event);
    void handleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void handleBoardSceneKeyEvent(QKeyEvent *event);
};

#endif // MAINWIDGET_H
