#include "AbstractBoard.h"

AbstractBoard::AbstractBoard(const Config *config) : config_(config)
{
    board_size_ = config->board_size;
    board_arr_.resize(board_size_);
    for (int i = 0; i < board_size_; ++i) {
        board_arr_[i].resize(board_size_);
    }
    tree_ = new ExplorerTree();
    cur_seq_len_ = 0;
}

void AbstractBoard::MakeMove(QPair<int, int> cell) {
    // $cell is not taken by one of the current stones
    if (cur_seq_len_ < sequence_.size() &&
            sequence_[cur_seq_len_] == cell) {
        ++cur_seq_len_;
    }
    else {
        while (sequence_.size() > cur_seq_len_) {
            sequence_.pop_back();
        }
        sequence_.push_back(cell);
        ++cur_seq_len_;
    }
    Cell new_cell = WHITESTONE;
    if (cur_seq_len_ % 2 == 0) {
        new_cell = BLACKSTONE;
    }
    board_arr_[cell.first][cell.second] = new_cell;
    tree_->MakeMove(cell);
}

QPair<int, int> AbstractBoard::GetLastMove() {
    return sequence_[cur_seq_len_ - 1];
}

bool AbstractBoard::Undo() {
   if (cur_seq_len_ > 0) {
       auto cell = sequence_[cur_seq_len_ - 1];
       board_arr_[cell.first][cell.second] = EMPTY;
       --cur_seq_len_;
       tree_->UndoLastMove();
       return true;
   }
   return false;
}

bool AbstractBoard::Redo() {
    if (cur_seq_len_ < sequence_.size()) {
        auto cell = sequence_[cur_seq_len_];
        MakeMove(cell);
        return true;
    }
    return false;
}

bool AbstractBoard::Empty() {
    return cur_seq_len_ == 0;
}

Cell AbstractBoard::GetCell(QPair<int, int> cell) {
    return board_arr_[cell.first][cell.second];
}

StoneColor AbstractBoard::GetCurrentColor() {
    if (cur_seq_len_ % 2 == 0) {
        return BLACK;
    }
    else {
        return WHITE;
    }
}

int AbstractBoard::MovesCount() {
    return cur_seq_len_;
}


bool AbstractBoard::IsCell(QPair<int, int> cell) {
    int i = cell.first, j = cell.second;
    return 0 <= i && i < board_size_ && 0 <= j && j < board_size_;
}

bool AbstractBoard::IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2) {
    int x = p1.first - p2.first;
    int y = p1.second - p2.second;
    return abs(x) == abs(y) || x * y == 0;
}
