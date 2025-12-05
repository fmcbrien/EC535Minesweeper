#pragma once

#include "GameBoard.h"
#include "CellButton.h"

#include <QMainWindow>
#include <vector>
#include <memory>

class QWidget;
class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void newGame();
    void onCellClicked(int row, int col);
    void onCellLongClicked(int row, int col);
    void onCellModelChanged(int row, int col);
    void onGameLost();
    void onGameWon();

private:
    QWidget* centralWidget;
    QGridLayout* gridLayout;
    std::unique_ptr<GameBoard> board;
    std::vector<std::vector<CellButton*>> buttons;
    void rebuildGrid(int rows, int cols);
};