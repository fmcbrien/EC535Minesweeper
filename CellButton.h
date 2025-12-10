#pragma once

#include <QPushButton>
#include <QTimer>

class CellButton : public QPushButton {
    Q_OBJECT
public:
    CellButton(int row, int col, QWidget* parent = nullptr);
    int rowCount() const { return row_; }
    int colCount() const { return col_; }
    void setNumber(int num);
    void setMine(bool mine);
    void setStateHidden();
    void setStateFlagged();
    void setStateRevealed(int adjacentMines);

    void setStateRevealedStyle();

signals:
    void clicked(int row, int col);         //short click = reveal tile
    void longClicked(int row, int col);     //long click = toggle flag

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void onLongClickTimeout();

private:
    int row_;
    int col_;
    QTimer longClickTimer;
    bool longClickDetected = false;
};