#include "DrillMainWidget.h"

DrillMainWidget::DrillMainWidget(Config *config, Settings *settings, QWidget *parent) : QWidget(parent)
{
    tools_.config = config;
    tools_.settings = settings;
    tools_.board = new AbstractBoard(config->board_size);
}
