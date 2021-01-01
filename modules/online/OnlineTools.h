#ifndef ONLINETOOLS_H
#define ONLINETOOLS_H

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
    AbstractBoard *board;
};


#endif // ONLINETOOLS_H
