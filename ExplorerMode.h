#ifndef EXPLORERMODE_H
#define EXPLORERMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
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
    ExplorerModeBase(ExplorerMode mode, BoardExplorerTools tools);

    virtual ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleKeyPressEvent(QKeyEvent *event);

    virtual ExplorerMode NbestValueChanged(int new_value);

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
    ExplorerMode mode_;
    BoardExplorerTools tools_;
};


class ExplorerModeDefault : public ExplorerModeBase {
public:
    ExplorerModeDefault(BoardExplorerTools tools);
    ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleKeyPressEvent(QKeyEvent *event);
};


class ExplorerModeDrawLine : public ExplorerModeBase {
public:
    ExplorerModeDrawLine(BoardExplorerTools tools);
    ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
};


#endif // EXPLORERMODE_H
