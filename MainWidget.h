#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QSet>
#include "AbstractBoard.h"
#include "BoardScene.h"
#include "BoardView.h"
#include "ExplorerMode.h"
#include "BoardPainter.h"
#include "BoardContextStorage.h"
#include "Config.h"
#include "Settings.h"
#include "EngineWrapper.h"
#include "BasicEngineViewer.h"
#include "BoardLayout.h"


class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Config *config_;
    Settings *settings_;

    BoardView *board_view_;
    BasicEngineViewer *engine_viewer_;

    BoardScene *board_scene_;
    AbstractBoard *board_;
    BoardPainter *painter_;
    BoardContextStorage *storage_;
    EngineWrapper *engine_wrapper_;

    ExplorerModeBase *current_mode_;
    ExplorerModeDefault *default_mode_;
    ExplorerModeDrawLine *draw_line_mode_;

    ExplorerModeBase *TranslateModeToPtr(ExplorerMode mode);

public:
    explicit MainWidget(QWidget *parent = nullptr);
    void handleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event);
    void handleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void handleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void handleBoardSceneKeyEvent(QKeyEvent *event);
};

#endif // MAINWIDGET_H
