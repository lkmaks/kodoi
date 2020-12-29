#ifndef DRILLTOOLS_H
#define DRILLTOOLS_H

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
#include "BoardDrillContextStorage.h"
#include "DrillInfoWidget.h"


struct BoardDrillTools {
    const Config *config;
    Settings *settings;
    AbstractBoard *board;
    BoardScene *board_scene;
    BoardPainter *painter;
    BoardDrillContextStorage *storage;
    DrillInfoWidget *info_widget;
};

#endif // DRILLTOOLS_H
