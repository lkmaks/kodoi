#ifndef EXPLORERMODE_H
#define EXPLORERMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
//#include "AbstractBoard.h"
//#include "BoardPainter.h"
//#include "BoardContextStorage.h"
//#include "Settings.h"
//#include "EngineWrapper.h"
#include "BoardTools.h"


enum class ExplorerMode {
    BASE,
    DEFAULT,
    DRAWLINE
};

class ExplorerModeBase : public QObject {

    Q_OBJECT

public:
    // auxiliary constructor to initialize different modes in derived classes using it
    ExplorerModeBase(ExplorerMode mode, BoardTools tools);

    virtual ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleKeyPressEvent(QKeyEvent *event);

    void MakeMove(QPair<int, int> cell);
    void Undo();
    void UndoUntil(QPair<int, int> cell);
    void Redo();
    void SetViewMarks(bool show);
    void StartPondering();
    void UpdatePonderingPosition();
    void StopPondering();

    // auxillary method: update next move marks on the board according to the tree
    void RenderMarks();

signals:
    void NeedRestartPondering();

protected:
    ExplorerMode mode_;

    const Config *config_;
    Settings *settings_;
    AbstractBoard *board_;
    BoardPainter *painter_;
    BoardContextStorage *storage_;
    EngineWrapper *engine_wrapper_;
};


class ExplorerModeDefault : public ExplorerModeBase {
public:
    ExplorerModeDefault(BoardTools tools);
    ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleKeyPressEvent(QKeyEvent *event);
};


class ExplorerModeDrawLine : public ExplorerModeBase {
public:
    ExplorerModeDrawLine(BoardTools tools);
    ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
};


#endif // EXPLORERMODE_H
