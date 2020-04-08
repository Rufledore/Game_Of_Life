#ifndef SIMULATIONRUNNER_H
#define SIMULATIONRUNNER_H

#include <QThread>


class SimulationRunner : public QThread
{
    Q_OBJECT
public:
    explicit SimulationRunner(QObject *parent = nullptr);


signals:

};

#endif // SIMULATIONRUNNER_H
