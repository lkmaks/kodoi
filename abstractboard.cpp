#include "abstractboard.h"

AbstractBoard::AbstractBoard(const Config &config) : config_(config)
{
    board_size_ = config.board_size;
    board_arr_.resize(board_size_);
    for (int i = 0; i < board_size_; ++i) {
        board_arr_[i].resize(board_size_);
    }
    tree_ = new ExplorerTree();
}

void AbstractBoard::MakeMove(QPair<int, int> cell) {
    // assume cell is not taken by one of the current stones
    int i = cell.first;
    int j = cell.second;
    position_.push_back({i, j});
    Cell new_cell = WHITESTONE;
    if (position_.size() % 2 == 0) {
        new_cell = BLACKSTONE;
    }
    board_arr_[i][j] = new_cell;
    tree_->MakeMove(cell);
}

QPair<int, int> AbstractBoard::GetLastMove() {
    return position_.back();
}

void AbstractBoard::UndoLastMove() {
   if (!position_.empty()) {
       int i = position_.back().first;
       int j = position_.back().second;
       board_arr_[i][j] = EMPTY;
       position_.pop_back();
       tree_->UndoLastMove();
   }
}

bool AbstractBoard::Empty() {
    return position_.empty();
}

Cell AbstractBoard::GetCell(QPair<int, int> cell) {
    return board_arr_[cell.first][cell.second];
}

StoneColor AbstractBoard::GetCurrentColor() {
    if (position_.size() % 2 == 0) {
        return BLACK;
    }
    else {
        return WHITE;
    }
}

bool AbstractBoard::IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2) {
    int x = p1.first - p2.first;
    int y = p1.second - p2.second;
    return abs(x) == abs(y) || x * y == 0;
}

int AbstractBoard::MovesCount() {
    return position_.size();
}

bool AbstractBoard::IsCell(QPair<int, int> cell) {
    int i = cell.first, j = cell.second;
    return 0 <= i && i < board_size_ && 0 <= j && j < board_size_;
}
