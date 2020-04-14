#include <QThread>

#include "simulationrunner.h"




void SimulationRunner::run()
{
    m_simulation->runningSimulation();

}

void SimulationRunner::startSimulation(SimulationCore* simulation)
{
    m_simulation = simulation;

    run();
}

