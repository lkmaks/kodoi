#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H

#include "Config.h"
#include "modules/Settings.h"
#include "common/BoardPainter.h"
#include "common/BoardLayout.h"
#include "common/EngineWrapper.h"
#include "common/EngineViewer.h"
#include "common/ColorBar.h"
#include "common/BoardScene.h"
#include "common/InfoWidget.h"
#include "common/BoardView.h"
#include "OnlineContextStorage.h"
#include "OnlineClient.h"
#include "ActionBoard.h"

struct BoardOnlineTools {
    const Config *config;
    Settings *settings;
    BoardScene *board_scene;
    BoardPainter *painter;
    EngineWrapper *engine_wrapper;
    ColorBar *color_bar;
    InfoWidget *info_widget;
    EngineViewer *engine_viewer;
    OnlineClient *client;
    OnlineContextStorage *storage;
    ActionBoard *action_board;
    AbstractBoard *board;
};


#endif // BOARDTOOLS_H
