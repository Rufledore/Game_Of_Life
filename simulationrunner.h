#ifndef SIMULATIONRUNNER_H
#define SIMULATIONRUNNER_H

#include <QThread>

#include "simulationcore.h"


class SimulationRunner : public QThread
{
    Q_OBJECT
public:
    void startSimulation(SimulationCore* simulation);

    SimulationCore* m_simulation;


public slots:

private slots:
    void run() override;

};

#endif // SIMULATIONRUNNER_H
