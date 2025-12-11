#include "CellButton.h"

#include <QMouseEvent>

// CREATE TILES THAT ARE 50 X 28 PIXELS
// THESE ARE THE TILES USED TO CREATE THE MINESWEEPER GRID
CellButton::CellButton(int row, int col, QWidget* parent)
    : QPushButton(parent), row_(row), col_(col) {
    setFixedSize(50, 28);
    setCheckable(false);
    longClickTimer.setInterval(600); // 600 ms for long press
    longClickTimer.setSingleShot(true);
    connect(&longClickTimer, &QTimer::timeout, this, &CellButton::onLongClickTimeout);
}

// MOUSE PRESS 
// START LONG CLCIK TIMER IF MOUSE BUTTON IS PRESSED
void CellButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        longClickDetected = false;
        longClickTimer.start();
    }
    QPushButton::mousePressEvent(event);
}

// MOUSE RELEASE 
// IF TIMER NOT TRIGGER THAN IT IS SHORT-CLICK
void CellButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        longClickTimer.stop();
        if (!longClickDetected) {
            emit clicked(row_, col_);
        }
    }
    QPushButton::mouseReleaseEvent(event);
}

// CHECK IF PRESS FOR 600MS PLACE
void CellButton::onLongClickTimeout() {
    longClickDetected = true;
    emit longClicked(row_, col_);
}

// DISPLAY NUMBER OF ADJACENT MINES
void CellButton::setNumber(int num) {
    setText(QString::number(num));
    setEnabled(false);
}

// DISPLAY A MINE X WHEN GAME LOST
void CellButton::setMine(bool mine) {
    if (mine) {
        setText("X");
    } else {
        setText("");
    }
    setEnabled(false);
}

// SET TILE TO HIDDEN STATE
void CellButton::setStateHidden() {
    setText("");
    setEnabled(true);
}

// SET TILE AS FLAGGED WITH CUSTOM ICON
void CellButton::setStateFlagged(const QIcon &icon) {
    setText("");
    setEnabled(true);
    setIcon(icon);
    setIconSize(QSize(20,20));
}

// SET TILE AS REVEALED AND POSSIBLY DISPLAY MINE
void CellButton::setStateRevealed(int adjacentMines) {
    if (adjacentMines > 0) {
        setText(QString::number(adjacentMines));
    } else {
        setText("");
    }
    setEnabled(false);
}

// WHEN TILE IS REVEALED 
void CellButton::setStateRevealedStyle(){
    //PREVENT RECLICKING
    setEnabled(false);

    // SET THE STYLE
    setStyleSheet(
        "QPushButton {"
        " background-color: #353535ff;"  
        " border: 1px solid #000000ff;"  
        " font-weight: bold;"
        "}"
    );
}