#ifndef DRILLMODE_H
#define DRILLMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "BoardTools.h"


enum class DrillMode {
    INACTIVE,
    PLAYERMOVE,
    COACHMOVE
};

class DrillModeBase : public QObject {

    Q_OBJECT

public:
    // auxiliary constructor to initialize different modes in derived classes using it
    DrillModeBase(DrillMode mode, BoardDrillTools tools);

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
    BoardExplorerTools tools_;
};


class DrillModeInactive : public DrillModeBase {
public:
    DrillModeInactive(BoardDrillTools tools);
};


class DrillModePlayerMove : public DrillModeBase {
public:
    DrillModePlayerMove (BoardDrillTools tools);
};

class DrillModeCoachMove : public DrillModeBase {
public:
    DrillModeCoachMove(BoardDrillTools tools);
};


#endif // DRILLMODE_H
