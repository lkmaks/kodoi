#ifndef EXPLORERMODE_H
#define EXPLORERMODE_H

#include <QString>
#include <QGraphicsSceneMouseEvent>
#include "abstractboard.h"
#include "boardpainter.h"
#include "commonmodedatastorage.h"


enum ExplorerMode {
    BASE,
    DEFAULT,
    DRAWLINE
};


class ExplorerModeBase {
public:
    // auxiliary constructor to initialize different modes in derived classes through it
    ExplorerModeBase(ExplorerMode mode, AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage);
    ExplorerModeBase(AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage);

    virtual ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual ExplorerMode HandleKeyPressEvent(QKeyEvent *event);

    void MakeMove(QPair<int, int> cell);
    void Undo();
    void UndoUntil(QPair<int, int> cell);
protected:
    ExplorerMode mode_;

    AbstractBoard *board_;
    BoardPainter *painter_;
    CommonModeDataStorage *storage_;
};


class ExplorerModeDefault : public ExplorerModeBase {
public:
    ExplorerModeDefault(AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage);
//    ExplorerModeDefault(ExplorerMode mode, BoardPainter *board_painter, AbstractBoard *board) :
//        ExplorerModeBase(mode, board_painter, board) {}
    ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    //ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
};


class ExplorerModeDrawLine : public ExplorerModeBase {
public:
    ExplorerModeDrawLine(AbstractBoard *board, BoardPainter *painter, CommonModeDataStorage *storage);
    //ExplorerMode HandleMousePressEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    ExplorerMode HandleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
};


#endif // EXPLORERMODE_H
