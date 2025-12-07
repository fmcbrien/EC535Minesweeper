#include "CellButton.h"

#include <QMouseEvent>

CellButton::CellButton(int row, int col, QWidget* parent)
    : QPushButton(parent), row_(row), col_(col) {
    setFixedSize(28, 28);
    setCheckable(false);
    longClickTimer.setInterval(600); // 600 ms for long press
    longClickTimer.setSingleShot(true);
    connect(&longClickTimer, &QTimer::timeout, this, &CellButton::onLongClickTimeout);
}

void CellButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        longClickDetected = false;
        longClickTimer.start();
    }
    QPushButton::mousePressEvent(event);
}

void CellButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        longClickTimer.stop();
        if (!longClickDetected) {
            emit clicked(row_, col_);
        }
    }
    QPushButton::mouseReleaseEvent(event);
}

void CellButton::onLongClickTimeout() {
    longClickDetected = true;
    emit longClicked(row_, col_);
}

void CellButton::setNumber(int num) {
    setText(QString::number(num));
    setEnabled(false);
}

void CellButton::setMine(bool mine) {
    if (mine) {
        setText("X");
    } else {
        setText("");
    }
    setEnabled(false);
}

void CellButton::setStateHidden() {
    setText("");
    setEnabled(true);
}

void CellButton::setStateFlagged() {
    setText("F");
    setEnabled(true);
}

void CellButton::setStateRevealed(int adjacentMines) {
    if (adjacentMines > 0) {
        setText(QString::number(adjacentMines));
    } else {
        setText("");
    }
    setEnabled(false);
}