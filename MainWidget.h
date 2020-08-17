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
#include "CommonModeDataStorage.h"
#include "Config.h"
#include "Settings.h"
#include "EngineWrapper.h"

class MainWidget : public QWidget
{
    Q_OBJECT

private:
    Config *config_;
    Settings *settings_;

    BoardScene *board_scene_;
    BoardView *board_view_;

    AbstractBoard *board_;
    BoardPainter *painter_;
    CommonModeDataStorage *storage_;

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
