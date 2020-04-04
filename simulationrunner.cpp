#include <QDebug>

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

    outputParameters.numberOfInfections = infectedPopulationMap->size();
    /*-------------------------------------------------------------------------------------------*/
    populationStatusUpdated(infectedPopulationMap.data());
    updatedOutputParameters(&outputParameters);

}

void SimulationRunner::UpdateInputParameters(InputPerameters* parameters)
{
    inputParameters = *parameters;
    qDebug() << inputParameters.deathRateMin;
}

OutputParameters SimulationRunner::getOutputParameters()
{
    return outputParameters; 
}

void SimulationRunner::changeClickedPersonState(CellCoordinates cell)
{
    Person* clickedPerson = &(*infectedPopulationMap)[cell]; // If the key is not in the map it is automatically generated.
    switch (clickedPerson->vitalityState) {
    case VitalityState::healty:
        ++outputParameters.numberOfInfections;
        break;
    case VitalityState::infected_incubation:
        break;
    case VitalityState::infected_sick:
        --outputParameters.numberOfInfections;
        ++outputParameters.numberOfDeaths;
        break;
    case VitalityState::dead:
        --outputParameters.numberOfDeaths;
        break;
    }

    clickedPerson->updateVitalityState();
    updatedOutputParameters(&outputParameters);
}
