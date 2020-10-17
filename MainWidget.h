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
#include "EngineViewer.h"
#include "BoardLayout.h"
#include "ColorBar.h"
#include "BoardTools.h"

#include <QTextEdit>


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(Settings *settings, QWidget *parent = nullptr);

    void AppSettingsUpdated(SettingsField field);

    // slots coming from user interaction

    // board
    void HandleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event);
    void HandleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void HandleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void HandleBoardSceneKeyEvent(QKeyEvent *event);

    // info widget

    void NbestValueChanged(int new_value);

private:
    Config *config_;
    Settings *settings_;

    // widgets
    BoardView *board_view_;
    ColorBar *color_bar_;
    InfoWidget *info_widget_;

    // logical board tools
    BoardScene *board_scene_;
    AbstractBoard *board_;
    BoardPainter *painter_;
    EngineWrapper *engine_wrapper_;
    BoardContextStorage *storage_;

    // High-entity managers
    EngineViewer *engine_viewer_;

    ExplorerModeBase *current_mode_;
    ExplorerModeDefault *default_mode_;
    ExplorerModeDrawLine *draw_line_mode_;

    // utility
    ExplorerModeBase *TranslateModeToPtr(ExplorerMode mode);
};

#endif // MAINWIDGET_H
