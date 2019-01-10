#ifndef RUNNINGGAMETHREAD_H
#define RUNNINGGAMETHREAD_H

#include "game.h"
#include <QThread>


class RunningGameThread : public QThread
{
    Q_OBJECT
public:
    void startGame(Game* game);
    void stopGame();

signals:
    void ready();
private:
    Game* game;

    void run() override;

};

#endif // RUNNINGGAMETHREAD_H
