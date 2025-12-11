#include "MainWindow.h"

#include <QApplication>

// INITIALIZE QT FRAMEWORK AND MAIN GAME WINDOW
int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();
    return a.exec();
}

