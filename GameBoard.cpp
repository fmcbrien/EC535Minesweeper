#include "GameBoard.h"

#include <cstdlib>
#include <random>
#include <queue>

// INITIALIZE BOARD USING RESET()
GameBoard::GameBoard(int rows, int cols, int mines, QObject* parent)
: QObject(parent) {reset(rows, cols, mines); }

// RESET BOARD
void GameBoard::reset(int rows, int cols, int mines) {
    // Initialize board dimensions and mine count but mines placed after first reveal
    R = rows;
    C = cols;
    M = mines;
    cellsRevealed = 0;
    grid.assign(R*C, Cell());
}

// RETURN CONST REF TO CELL
const Cell& GameBoard::at(int row, int col) const {
    return grid[index(row, col)];
}
// RETURN NUM OF ROWS
int GameBoard::rowCount() const {
    return R;
}
// RETURN NUM OF COLS
int GameBoard::colCount() const {
    return C;
}

// RANDOMLY PLACE MINES
// MAKE SURE FIRST CLICK IS NOT A MINE
void GameBoard::placeMines(int initialRow, int initialCol) {
    std::vector<int> positions;
    positions.reserve(R * C);

    // GENERATE ALL CELL POSITIONS EXCEPT FIRST CLICKED CELL
    for (int i = 0; i < R * C; ++i) {
        int r = i / C;
        int c = i % C;
        if (r == initialRow && c == initialCol) continue; // Avoid placing mine on first revealed cell
        positions.push_back(i);
    }

    // RAND NUM GENERATOR
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(positions.begin(), positions.end(), g);

    // PLACE MINES IN SHUFFLE
    for (int i = 0; i < M; ++i) {
        grid[positions[i]].hasMine = true;
    }
    // COMPUTE MINES PLACED
    calculateAdjacents();
}

// CAL NUM OF MINES IN EACH CELL
void GameBoard::calculateAdjacents() {
    // DIRECT VECTORS FOR 8 ADJACENT CELLS
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[index(r, c)].hasMine) continue;
            int count = 0;
            for (int d = 0; d < 8; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[index(nr, nc)].hasMine) {
                    count++;
                }
            }
            grid[index(r, c)].adjacentMines = count;
        }
    }
}

// FLOOD FILL ALGORITHM WHICH REVEALS ALL EMPTY TILES
void GameBoard::revealAdjacentCells(int row, int col) {
    std::queue<std::pair<int, int>> q;
    q.push({row, col});

    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int d = 0; d < 8; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                Cell &neighbor = grid[index(nr, nc)];
                if (neighbor.state == CellState::Hidden && !neighbor.hasMine) {
                    neighbor.state = CellState::Revealed;
                    cellsRevealed++;
                    emit cellChanged(nr, nc);
                    if (neighbor.adjacentMines == 0) {
                        q.push({nr, nc});
                    }
                }
            }
        }
    }
}

// REVEAL A SINGLE CELL IF CLICKED
// THIS WILL ALSO HANDLE MINE DETECTION, FLOOD-FILL
// AND WINS OR LOSSES
bool GameBoard::revealCell(int row, int col) {
    Cell &cell = grid[index(row, col)];
    if (cell.state != CellState::Hidden) return true; // Ignore if already revealed or flagged

    // Place mines on first reveal
    if (cellsRevealed == 0) {
        placeMines(row, col);
    }

    if (cell.hasMine) {
        cell.state = CellState::Revealed;
        emit cellChanged(row, col);
        emit gameLost();
        return false; // Game over
    }

    cell.state = CellState::Revealed;
    cellsRevealed++;
    emit cellChanged(row, col);

    if (cell.adjacentMines == 0) {
        revealAdjacentCells(row, col);
    }

    if (isGameWon()) {
        emit gameWon();
    }

    return true;
}

// TOGGLE FLAG BY LONG CLICKING
void GameBoard::toggleFlag(int row, int col) {
    Cell &cell = grid[index(row, col)];
    if (cell.state == CellState::Revealed) return; // Can't flag revealed cells

    if (cell.state == CellState::Hidden) {
        cell.state = CellState::Flagged;
    } else if (cell.state == CellState::Flagged) {
        cell.state = CellState::Hidden;
    }
    emit cellChanged(row, col);
}

// WIN CONDITION
bool GameBoard::isGameWon() const {
    return (R * C - cellsRevealed) == M;
}