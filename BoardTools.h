#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H

#include "Config.h"
#include "Settings.h"
#include "AbstractBoard.h"
#include "BoardPainter.h"
#include "BoardContextStorage.h"
#include "EngineWrapper.h"
#include "ColorBar.h"

struct BoardTools {
    const Config *config;
    Settings *settings;
    AbstractBoard *board;
    BoardPainter *painter;
    BoardContextStorage *storage;
    EngineWrapper *engine_wrapper;
    ColorBar *color_bar;
};

#endif // BOARDTOOLS_H
