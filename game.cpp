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
//    connect(this, &Game::keyClicked, this, &Game::startGame);
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
        QThread::msleep(10);
//        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        qDebug() << i++;
        QCoreApplication::processEvents();                      //Continue with processing after all thread complete
    }
}

void Game::startGame(int incubationPeriod, int illnessPeriod, int infectionRate, double deathRate)
{
    qDebug() << gameIsStarted;

    if (gameIsStarted) {
        QScopedPointer<RunningGameThread> gameThread(new RunningGameThread);
//        RunningGameThread* gameThread = new RunningGameThread();
        gameThread->startGame(this);

        connect(gameThread.data(), &RunningGameThread::finished, gameThread.data(), &QObject::deleteLater);
    }
}
