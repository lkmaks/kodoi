#ifndef EXPLORERMODE_H
#define EXPLORERMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include "abstractboard.h"
#include "boardpainter.h"
#include "commonmodedatastorage.h"

enum ExplorerMode {
    DEFAULT,
    DRAWLINE
};

class ExplorerModeBase {
public:
    ExplorerModeBase(BoardPainter *board_painter, AbstractBoard *board) :
        board_painter_(board_painter), board_(board) {}

    virtual ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleKeyPressEvent(QKeyEvent *event);

    void AddStone(int i, int j);
    void Undo();
    void UndoUntil(int i, int j);
protected:
    BoardPainter *board_painter_;
    AbstractBoard *board_;
    CommonModeDataStorage *storage_;
};

class ExplorerModeDefault : public ExplorerModeBase {
public:
    ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleKeyPressEvent(QKeyEvent *event);
};

class ExplorerModeDrawLine : public ExplorerModeBase {

};

#endif // EXPLORERMODE_H
