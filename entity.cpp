#include "entity.h"
#include <QWidget>


Entity::Entity (QWidget *parent) :
        QLabel (parent),
        currentState (Dead)
{
    syncBitmap();
}

void Entity::setState(State newState)
{
    if (currentState != newState) {
        currentState = newState;
        syncBitmap();
    }
}

void Entity::syncBitmap()
{
    QPixmap dead(":/Resources/Dead-Entity.png");
//    QPixmap live();

    switch (currentState) {
//    case Live: setText("U"); break;
//    case Dead: setText("U"); break;
    case Live: setPixmap(dead.scaled(10, 10)); break;
    case Dead: setPixmap(dead.scaled(10, 10)); break;
    }
}

Entity::State Entity::getState() const
{
    return currentState;
}

