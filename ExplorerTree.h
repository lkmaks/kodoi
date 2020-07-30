#ifndef EXPLORERTREE_H
#define EXPLORERTREE_H

#include <QVector>
#include <QGraphicsItem>
#include "Enums.h"


class ExplorerTree
{
public:
    struct NodeInfo;
    struct Node;

    ExplorerTree();

    void MakeMove(QPair<int, int> cell);
    void UndoLastMove();
    void UndoUntil(QPair<int, int> cell);

    Node *GetCurrentNode();

    struct NodeInfo {
        //NodeInfo();
    };

    struct Node {
        Node(Node *parent, StoneColor color, QPair<int, int> cell);
        Node();

        // structure
        QMap<QPair<int, int>, Node*> children;
        Node *parent;

        // primary characteristics
        StoneColor color;
        QPair<int, int> cell;

        // additional info that will be used for painting the marks
        NodeInfo info;
    };
private:
    Node *root_;
    Node *cur_node_;
};

#endif // EXPLORERTREE_H
