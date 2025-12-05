cmake_minimum_required(VERSION 3.5)
project(MinesweeperQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/GameBoard.cpp
    src/CellButton.cpp
)

set(HEADERS
    src/MainWindow.h
    src/GameBoard.h
    src/CellButton.h
)

add_executable(minesweeper ${SOURCES} ${HEADERS})
target_link_libraries(minesweeper PRIVATE Qt5::Widgets)
