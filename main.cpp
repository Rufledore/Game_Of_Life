#include <QApplication>
#include "mainwindow.h"
#include "populationmap.h"
#include "game.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
//    Game game;
//    game.show();

    return a.exec();
}
