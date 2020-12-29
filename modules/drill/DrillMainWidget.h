#ifndef DRILLMAINWIDGET_H
#define DRILLMAINWIDGET_H

#include <QWidget>
#include "DrillTools.h"
#include "DrillMode.h"

class DrillMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrillMainWidget(Config *config, Settings *settings, QWidget *parent = nullptr);

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
    DrillTools tools_;

    DrillModeInactive *inactive_mode_;
    DrillModePlayerMove *playermove_mode_;
    DrillModeCoachMove *coachmove_mode_;

    DrillModeBase *current_mode_;

    // utility
    DrillModeBase *TranslateModeToPtr(DrillMode mode);
};

#endif // DRILLMAINWIDGET_H
