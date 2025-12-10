# EC535Minesweeper

## Introduction

This is an implementation of Minesweeper developed as a userspace game for the final project of EC535. This game was developed using Qt 5 and C++ as per the requirements and is made to be implemented on a Beaglebone with an LCD screen.

## Setup

1. Dependencies
  - Qt 5
  - C++

2. Build the project
   - `qmake`
   - `make`
   - upload minesweeper file to beaglebone
3. Run Game
   - `./minesweeper -platform linuxfb:fb=/dev/fb0`

## Folder Contents/Structure

  - main.cpp
  - MainWindow.cpp
  - GameBoard.cpp
  - CellButton.cpp
  
  - minesweeper_bomb.png
  - minesweeper_flag.png
