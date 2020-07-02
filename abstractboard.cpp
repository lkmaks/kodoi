#include "abstractboard.h"

AbstractBoard::AbstractBoard(const Config &config) : config_(config)
{
    board_size_ = config.board_size;
    board_arr.resize(board_size_);
    for (int i = 0; i < board_size_; ++i) {
        board_arr[i].resize(board_size_);
    }
}

void AbstractBoard::MakeMove(int i, int j) {
    position.push_back({i, j});
    Cell cell = WHITESTONE;
    if (position.size() % 2 == 0) {
        cell = BLACKSTONE;
    }
    board_arr[i][j] = cell;
}

QPair<int, int> AbstractBoard::GetLastMove() {
    return position.back();
}

void AbstractBoard::UndoLastMove() {
   if (!position.empty()) {
       int i = position.back().first;
       int j = position.back().second;
       board_arr[i][j] = EMPTY;
       position.pop_back();
   }
}

bool AbstractBoard::Empty() {
    return position.empty();
}

Cell AbstractBoard::GetCell(int i, int j) {
    return board_arr[i][j];
}

StoneColor AbstractBoard::GetCurrentColor() {
    if (position.size() % 2 == 0) {
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
    return position.size();
}

bool AbstractBoard::IsCell(QPair<int, int> cell) {
    int i = cell.first, j = cell.second;
    return 0 <= i && i < board_size_ && 0 <= j && j < board_size_;
}
