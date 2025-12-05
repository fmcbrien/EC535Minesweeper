#pragma once

#include <vector>
#inlcude <functional>

enum class CellState {
    Hidden,
    Revealed,
    Flagged
};

struct Cell {
    bool hasMine = false;
    int adjacentMines = 0;
    CellState state = CellState::Hidden;
};

class GameBoard : public QObject {
    Q_OBJECT
public:
    GameBoard(int rows = 9, int cols = 9, int mines = 10, QObject* parent = nullptr);
    void reset(int rows, int cols, int mines);
    bool revealCell(int row, int col);  //reutrns false if mine is revealed -> game over
    void toggleFlag(int row, int col);
    const Cell& at(int row, int col) const;
    int rowCount() const;
    int colCount() const;
    bool isGameWon() const;

signals:
    void cellChanged(int row, int col);
    void gameLost();
    void gameWon();

private:
    int R;
    int C;
    int M;
    int cellsRevealed = 0;
    std::vector<std::vector<Cell>> grid;
    void placeMines(int initialRow, int initialCol);
    void calculateAdjacents();
    void revealAdjacentCells(int row, int col);
    inline int index(int row, int col) const {
        return row * C + col;
    }
};