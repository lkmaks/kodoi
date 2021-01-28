#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QSet>
#include <QTextEdit>


#include "OnlineTools.h"
#include "OnlineMode.h"
#include "OnlineContextStorage.h"


class OnlineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineWidget(Config *config, Settings *settings, QWidget *parent = nullptr);

    void AppSettingsUpdated(SettingsField field);

    void OnlineRoomCreate(RoomId room_id);
    void OnlineRoomEnter(RoomId room_id);

    // slots coming from user interaction

    // board
    void HandleBoardSceneMousePressEvent(QGraphicsSceneMouseEvent *event);
    void HandleBoardSceneMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void HandleBoardSceneMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void HandleBoardSceneKeyPressEvent(QKeyEvent *event);

    // info widget
    void NbestValueChanged(int new_value);

    // online
    void HandleOnlineReceivedStatus(bool status);
    void HandleOnlineReceivedInit(BoardAction action);
    void HandleOnlineReceivedUpdate(BoardAction action);

private:
    Config *config_;
    Settings *settings_;

    // widgets
    BoardView *board_view_;
    ColorBar *color_bar_;
    InfoWidget *info_widget_;

    // logical board tools

    AbstractBoard *board_;
    OnlineBoard *online_board_;
    BoardScene *board_scene_;
    BoardPainter *painter_;
    EngineWrapper *engine_wrapper_;
    OnlineContextStorage *storage_;

    // Online session
    OnlineSession *client_;

    // managers
    EngineViewer *engine_viewer_;

    OnlineModeBase *current_mode_;
    OnlineModeDefault *default_mode_;
    OnlineModeDrawLine *draw_line_mode_;

    // utility
    OnlineModeBase *TranslateModeToPtr(OnlineMode mode);
};

#endif // ONLINEWIDGET_H
