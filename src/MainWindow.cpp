#include "MainWindow.h"

#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    board = std::make_unique<GameBoard>(9, 9, 10, this);
    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(4);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    auto fileMenu = menuBar()->addMenu("&Game");
    QAction* newGameAction = new QAction("&New", this);
    fileMenu->addAction(newGameAction);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    statusBar()->showMessage("Welcome to Minesweeper!");

    connect(board.get(), &GameBoard::cellChanged, this, &MainWindow::onCellModelChanged);
    connect(board.get(), &GameBoard::gameLost, this, &MainWindow::onGameLost);
    connect(board.get(), &GameBoard::gameWon, this, &MainWindow::onGameWon);

    rebuildGrid(board->rowCount(), board->colCount());
}

void MainWindow::rebuildGrid(int rows, int cols) {
    // Clear existing buttons
    qDeleteAll(buttons);
    buttons.clear();

    // Create new buttons
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            CellButton* btn = new CellButton(r, c, centralWidget);
            gridLayout->addWidget(btn, r, c);

            connect(btn, &CellButton::clicked, this, &MainWindow::onCellClicked);
            connect(btn, &CellButton::longClicked, this, &MainWindow::onCellLongClicked);

            buttons.push_back(btn);
        }
    }
    adjustSize();
}

void MainWindow::newGame() {
    board->reset(9, 9, 10);
    rebuildGrid(board->rowCount(), board->colCount());
    statusBar()->showMessage("New game started!");
}

void MainWindow::onCellClicked(int row, int col) {
    board->revealCell(row, col);
}

void MainWindow::onCellLongClicked(int row, int col) {
    board->toggleFlag(row, col);
}

void MainWindow::onCellModelChanged(int row, int col) {
    const Cell &cell = board->at(row, col);
    CellButton* btn = buttons[row * board->colCount() + col];
    if (cell.state == CellState::Hidden) btn->setStateHidden();
    else if (cell.state == CellState::Flagged) btn->setStateFlagged();
    else if (cell.state == CellState::Revealed) {
        if (cell.hasMine) btn->setMine(true);
        else btn->setStateRevealed(cell.adjacentMines);
    }
}

void MainWindow::onGameLost() {
    statusBar()->showMessage("Game over! You lost.");
    QMessageBox::information(this, "Game Over", "You hit a mine.");
}

void MainWindow::onGameWon() {
    statusBar()->showMessage("Congratulations! You won.");
    QMessageBox::information(this, "Congratulations", "You cleared the minefield!");
}