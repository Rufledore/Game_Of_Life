#ifndef ENTITY_H
#define ENTITY_H

#include <QWidget>
#include <QLabel>


class Entity : public QLabel
{

    Q_OBJECT

public:

    enum State { Live, Dead };

    Entity (QWidget *parent = nullptr);

    State getState() const;
    void setState(State newState);

private:
    void syncBitmap();

    State currentState;
};

#endif // ENTITY_H
