#ifndef OnlineMODE_H
#define OnlineMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "OnlineTools.h"
#include "OnlineContextStorage.h"


enum class OnlineMode {
    BASE,
    DEFAULT,
    DRAWLINE
};

class OnlineModeBase : public QObject {

    Q_OBJECT

public:
    // auxiliary constructor to initialize different modes in derived classes using it
    OnlineModeBase(OnlineMode mode, BoardOnlineTools tool);

    virtual OnlineMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual OnlineMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual OnlineMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual OnlineMode HandleKeyPressEvent(QKeyEvent *event);

    virtual OnlineMode NbestValueChanged(int new_value);

    void MakeMove(QPair<int, int> cell);
    void Undo();
    void UndoUntil(QPair<int, int> cell);
    void Redo();

    void SetViewMarks(bool show);
    void SetLastMoveHighlight(bool show);

    void StartPondering();
    void UpdatePonderingPosition(int nbest_value); // by board
    void StopPondering();
    //update next move marks on the board according to the tree
    void RenderMarks();
    void RenderLastMoveHighlight();
    void RenderAuxiliary();


signals:
    void NeedRestartPondering();

protected:
    OnlineMode mode_;
    BoardOnlineTools tools_;
};


class OnlineModeDefault : public OnlineModeBase {
public:
    OnlineModeDefault(BoardOnlineTools tools);
    OnlineMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    OnlineMode HandleKeyPressEvent(QKeyEvent *event);
};


class OnlineModeDrawLine : public OnlineModeBase {
public:
    OnlineModeDrawLine(BoardOnlineTools tools);
    OnlineMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    OnlineMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
};


#endif // OnlineMODE_H
