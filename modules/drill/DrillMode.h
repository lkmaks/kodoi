#ifndef DRILLMODE_H
#define DRILLMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "DrillTools.h"


enum class DrillMode {
    INACTIVE,
    PLAYERMOVE,
    COACHMOVE
};

class DrillModeBase : public QObject {

    Q_OBJECT

public:
    // auxiliary constructor to initialize different modes in derived classes using it
    DrillModeBase(DrillMode mode, DrillTools tools);

//    virtual DrillMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
//    virtual DrillMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
//    virtual DrillMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    virtual DrillMode HandleKeyPressEvent(QKeyEvent *event);

    void MakeMove(QPair<int, int> cell);
    void Undo();
    void UndoUntil(QPair<int, int> cell);
    void Redo();

    void SetViewMarks(bool show);
    void SetLastMoveHighlight(bool show);

    void RenderMarks();
    void RenderLastMoveHighlight();
    void RenderAuxiliary();


signals:
    void NeedRestartPondering();

protected:
    DrillMode mode_;
    DrillTools tools_;
};


class DrillModeInactive : public DrillModeBase {
public:
    DrillModeInactive(DrillTools tools);
};


class DrillModePlayerMove : public DrillModeBase {
public:
    DrillModePlayerMove (DrillTools tools);
};

class DrillModeCoachMove : public DrillModeBase {
public:
    DrillModeCoachMove(DrillTools tools);
};


#endif // DRILLMODE_H
