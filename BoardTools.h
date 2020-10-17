#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H

#include "Config.h"
#include "Settings.h"
#include "AbstractBoard.h"
#include "BoardPainter.h"
#include "BoardContextStorage.h"
#include "EngineWrapper.h"
#include "ColorBar.h"
#include "BoardScene.h"
#include "InfoWidget.h"
#include "BoardDrillContextStorage.h"
#include "BoardView.h"
#include "DrillInfoWidget.h"

struct BoardExplorerTools {
    const Config *config;
    Settings *settings;
    AbstractBoard *board;
    BoardScene *board_scene;
    BoardPainter *painter;
    BoardContextStorage *storage;
    EngineWrapper *engine_wrapper;
    ColorBar *color_bar;
    InfoWidget *info_widget;
};

struct BoardDrillTools {
    const Config *config;
    Settings *settings;
    AbstractBoard *board;
    BoardScene *board_scene;
    BoardPainter *painter;
    BoardDrillContextStorage *storage;
    DrillInfoWidget *info_widget;
};

#endif // BOARDTOOLS_H
