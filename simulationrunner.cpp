#include <QDebug>

#include "simulationrunner.h"

SimulationRunner::SimulationRunner(QObject *parent) :
    QObject(parent),
    infectedPopulationMap(new InfectionMap)
{
}

void SimulationRunner::updateInfectionsForTheDay()
{
    // Get the coordinates (hash keys) of the people who will spread the infection today.
    keysOfInfectors.clear();
    keysOfInfectors = infectedPopulationMap->keys();

    QList<CellCoordinates>::iterator infectorKey = keysOfInfectors.begin();
    QList<CellCoordinates>::iterator infectedPersonCoordinates = nullptr;
    while (infectorKey != keysOfInfectors.end()) {
        Person infector = infectedPopulationMap->value(*infectorKey);
        QList<CellCoordinates> infectedPeopleCoordinates = findPeopleInfectedBy(&infector);
        for (infectedPersonCoordinates = infectedPeopleCoordinates.begin();
             infectedPersonCoordinates != infectedPeopleCoordinates.end();
             ++infectedPersonCoordinates) {
            if (!infectedPopulationMap->contains(*infectedPersonCoordinates)){
                Person infectedPerson;
//                infectedPerson.getInfected()
                *infectedPopulationMap->insert(*infectedPersonCoordinates, infectedPerson);

            }
        }

        // This row deletes the value where the iterator is and returns an iterator to the next element.
        infectorKey = keysOfInfectors.erase(infectorKey);
    }


}

QList<CellCoordinates> SimulationRunner::findPeopleInfectedBy(const Person* infector)
{
    QList<CellCoordinates> infected;
    double probabilityToInfect = infector->getProbabilityToInfect();


    for (int i = 0; i < 100; ++i) {
        if (randomGenerator.generateUniform(0, 1) < probabilityToInfect) {
            infected.append(CellCoordinates(
                                static_cast<int>(randomGenerator.generateUniform(0, peoplePerRow)),   // In this way there is a very little lower chance for the last of the row to appear.
                                static_cast<int>(randomGenerator.generateUniform(0, peoplePerRow))));

            // Make the probability lower.
            probabilityToInfect *= probabilityToInfect;
        } else {
            // Experimental!
            // More people could be infected at once only in sequence. Every next one with lower probability.
            break;
        }
    }

    return infected;
}

void SimulationRunner::updateMap()
{
/*------------------------------------------------------------------------------------*/
    InputPerameters parameters;
    RandomGenerator rand;
    infectedPopulationMap->insert(CellCoordinates(0,1), Person());
    (*infectedPopulationMap.data())[CellCoordinates(0,1)].getInfected(parameters, rand);
    infectedPopulationMap->insert(CellCoordinates(100,100), Person());
    (*infectedPopulationMap.data())[CellCoordinates(100,100)].getInfected(parameters, rand);
    infectedPopulationMap->insert(CellCoordinates(1,1), Person());
    (*infectedPopulationMap.data())[CellCoordinates(1,1)].getInfected(parameters, rand);
    infectedPopulationMap->insert(CellCoordinates(200,129), Person());
    (*infectedPopulationMap.data())[CellCoordinates(200,129)].getInfected(parameters, rand);

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
