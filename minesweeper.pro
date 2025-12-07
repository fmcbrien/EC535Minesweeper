QT += widgets
CONFIG += release c++17
TEMPLATE = app
TARGET = minesweeper

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    GameBoard.cpp \
    CellButton.cpp

HEADERS += \
    MainWindow.h \
    GameBoard.h \
    CellButton.h
