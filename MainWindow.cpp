#include "MainWindow.h"

#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

// INCLUDE FOR ICONS
#include <QPixmap>
#include <QIcon>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    board = std::make_unique<GameBoard>(9, 9, 10, this);
    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(0);
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

    bombIcon = QIcon("minesweeper_bomb.png");
    flagIcon = QIcon("minesweeper_flag.png");
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

    // CLEAR ICONS
    btn->setIcon(QIcon());
    btn->setText("");
    
    // SET THE REVEALED CELL TO A DARKEN STYLE 
    btn->setStateRevealedStyle();

    // CHECK IF FLAGGED
    // IF SO ADD THE CUSTOM FLAG ICON
    if (cell.state == CellState::Flagged){
        QPixmap flag("minesweeper_flag.png");

        btn->setIcon(QIcon(flag));
        btn->setIconSize(QSize(20, 20));

        btn->setStyleSheet(
            "QPushButton {"
            "background-color: #C0C0C0;"
            "border: 1px solid #707070;"
            "padding: 0px;"
            "}"
        );
        return;
    }

    // CHECK IF MINE
    // IF SO ADD THE CUSTOM MINE ICON
    if (cell.hasMine){
        QPixmap bomb("minesweeper_bomb.png");

        btn->setText("");
        btn->setIcon(QIcon(bomb));
        btn->setIconSize(QSize(20,20));

        btn->setStyleSheet(
            "QPushButton {"
            "background-color: #808080;"
            "border: 1px solid #505050;"
            "padding: 0px;"
            "}"
        );
        return;
    }

    if (cell.state == CellState::Hidden) btn->setStateHidden();
    else if (cell.state == CellState::Flagged) btn->setStateFlagged(QIcon("minesweeper_flag.png"));
    else if (cell.state == CellState::Revealed) {
        if (cell.hasMine) btn->setMine(true);
        else btn->setStateRevealed(cell.adjacentMines);
    }

    // IF TILE IS MORE THAN 0, SET A COLOR ACCORDING TO NUM
    if (cell.adjacentMines > 0){
        btn->setText(QString::number(cell.adjacentMines));

        QString color;
        
        switch (cell.adjacentMines){
        case 1: color = "blue"; break;
        case 2: color = "green"; break;
        case 3: color = "red"; break;
        case 4: color = "purple"; break;
        default: color = "black";
    }

    btn->setStyleSheet(QString(
        "QPushButton {"
        "background-color: #A0A0A0;"
        "border: 1px solid #707070;"
        "color: %1;"
        "font-weight: bold;"
        "}"
    ).arg(color));
    }
    else {
        btn->setText("");
        btn->setStyleSheet(
            "QPushButton {"
            "background-color: #A0A0A0;" 
            "border: 1px solid #707070;"
            "color: black;"
            "}"
        );
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