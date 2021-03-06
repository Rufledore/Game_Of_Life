#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QKeyEvent>
#include "gamemap.h"
#include <thread>

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

    void keyPressEvent(QKeyEvent *event);
    void runningGame();
signals:
    void keyClicked();
public slots:
    void startGame();
private:
    bool gameIsStarted;
    GameMap *map;
    QKeyEvent *keyEvent;
    std::thread game;
};

#endif // GAME_H
