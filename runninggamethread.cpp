#include "runninggamethread.h"
#include "game.h"


void RunningGameThread::startGame(Game* game)
{
    this->game = game;
    run();
}

void RunningGameThread::run()
{
    game->runningGame();

    emit ready();
}
