#include "ExplorerTree.h"

ExplorerTree::Node::Node(Node *parent, StoneColor color, QPair<int, int> cell) :
    parent(parent), color(color), cell(cell) {}

ExplorerTree::Node::Node() :
    parent(nullptr), color(NONE), cell(QPair<int, int>(-1, -1)) {}

ExplorerTree::ExplorerTree()
{
    root_ = new Node();
    cur_node_ = root_;
}

void ExplorerTree::MakeMove(QPair<int, int> cell) {
    auto it = cur_node_->children.find(cell);
    if (it != cur_node_->children.end()) {
        cur_node_ = it.value();
    }
    else {
        Node *new_node = new Node(cur_node_, opposite_color(cur_node_->color), cell);
        cur_node_->children[cell] = new_node;
        cur_node_ = new_node;
    }
}

void ExplorerTree::UndoLastMove() {
    if (cur_node_->parent != nullptr) {
        // can undo a move
        cur_node_ = cur_node_->parent;
    }
}

void ExplorerTree::UndoUntil(QPair<int, int> cell) {
    while (cur_node_->cell != cell) {
        cur_node_ = cur_node_->parent;
    }
}

ExplorerTree::Node *ExplorerTree::GetCurrentNode() {
    return cur_node_;
}
