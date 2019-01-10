#include "game.h"
#include "gamemap.h"
#include "runninggamethread.h"
#include <QTimer>
#include <thread>
#include <QDebug>
#include <QCoreApplication>

Game::Game(QWidget *parent) :
    QWidget(parent),
    gameIsStarted(false),
    map(new GameMap(this)),
    keyEvent(nullptr)
{
//    GameMap *map = new GameMap(this);
    this->show();
    connect(this, &Game::keyClicked, this, &Game::startGame);
}

Game::~Game()
{
    game.join();
}

void Game::keyPressEvent(QKeyEvent *event)
{
//  qDebug() << event->key();
    gameIsStarted = !gameIsStarted;
    keyEvent = event;
    keyClicked();
}

void Game::runningGame()
{
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    while (gameIsStarted)
    {
        int i;
        map->updateNextGeneration();
        this->show();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        qDebug() << i++;
        QCoreApplication::processEvents();                      //Continue with processing after all thread complete
    }
}

void Game::startGame()
{
    qDebug() << gameIsStarted;
    /*
    while(true){
        qDebug() << i++;
        map->updateNextGeneration();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
    }

    */
    if (gameIsStarted) {
        RunningGameThread* gameThread = new RunningGameThread();
        gameThread->startGame(this);

        connect(gameThread, &RunningGameThread::finished, gameThread, &QObject::deleteLater);

    }


}
