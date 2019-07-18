#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include "boardscene.h"
#include "boardview.h"

class MainWidget : public QWidget
{
    Q_OBJECT

    friend BoardView;

private:
    BoardScene *board_scene;
    BoardView *board_view;

    int cell_size = 30;
    int board_size = 15;
    int stone_gap_factor = 10;
    QString current_mode;
    int step = 0;

    QPair<int, int> line_point_a, line_point_b;
    QVector<QGraphicsItem*> lines;

    QVector<QVector<int> > board_arr;
    QVector<QPair<int, int> > position;
    QVector<QGraphicsItem*> stones;
    QVector<QGraphicsItem*> numbers;

    void init_board_graphics();
    void init_board_alg();

    void handleLeftClickOn(int i, int j);
    void handleRightClickOn(int i, int j);

    void addStone(int i, int j);
    void undo_until(int i, int j);
    void undo();

    void putStone(int i, int j, int color, int number);
    void drawLineAB();
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void handleBoardSceneMouseEvent(QGraphicsSceneMouseEvent *event);
    void handleBoardSceneKeyEvent(QKeyEvent *event);
};

#endif // MAINWIDGET_H
