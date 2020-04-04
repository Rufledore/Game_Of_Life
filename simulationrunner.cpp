#include "simulationrunner.h"

SimulationRunner::SimulationRunner(QObject *parent) :
    QObject(parent),
    infectedPopulationMap(new InfectionMap)
{
}

void SimulationRunner::updateMap()
{
/*------------------------------------------------------------------------------------*/
    infectedPopulationMap->insert(CellCoordinates(0,1), Person());
    (*infectedPopulationMap.data())[CellCoordinates(0,1)].infect(20, 0.01);
    infectedPopulationMap->insert(CellCoordinates(100,100), Person());
    (*infectedPopulationMap.data())[CellCoordinates(100,100)].infect(20, 0.01);
    infectedPopulationMap->insert(CellCoordinates(1,1), Person());
    (*infectedPopulationMap.data())[CellCoordinates(1,1)].infect(20, 0.01);
    infectedPopulationMap->insert(CellCoordinates(200,129), Person());
    (*infectedPopulationMap.data())[CellCoordinates(200,129)].infect(20, 0.01);

//    populationMap->drawNextGeneration(infectedPopulationMap.data());

    m_numberOfInfected = infectedPopulationMap->size();
    /*-------------------------------------------------------------------------------------------*/
    populationStatusUpdated(infectedPopulationMap.data());

}

