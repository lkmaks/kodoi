#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H

#include "Config.h"
#include "modules/Settings.h"
#include "common/AbstractBoard.h"
#include "common/BoardPainter.h"
#include "common/BoardLayout.h"
#include "common/EngineWrapper.h"
#include "common/EngineViewer.h"
#include "common/ColorBar.h"
#include "common/BoardScene.h"
#include "common/InfoWidget.h"
#include "common/BoardView.h"
#include "OnlineContextStorage.h"

struct BoardOnlineTools {
    const Config *config;
    Settings *settings;
    AbstractBoard *board;
    BoardScene *board_scene;
    BoardPainter *painter;
    EngineWrapper *engine_wrapper;
    ColorBar *color_bar;
    InfoWidget *info_widget;
    EngineViewer *engine_viewer;
    OnlineContextStorage *storage;
};


#endif // BOARDTOOLS_H
