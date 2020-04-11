#include <QDebug>
#include <QThread>

#include "simulationcore.h"

SimulationCore::SimulationCore(QObject *parent) :
    QObject(parent),
    infectedPopulationMap(new InfectionMap),
    recoveredPopulationMap(new InfectionMap)
{
}

void SimulationCore::updateInfectionsForTheDay()
{
    //ONLY SICK PEOPLE SHOULD BE ADDED HERE
    // Get the coordinates (hash keys) of the people who will spread the infection today.
    keysOfInfectors.clear();
    keysOfInfectors = infectedPopulationMap->keys();


    QList<CellCoordinates>::iterator infectorKey = keysOfInfectors.begin();
    QList<CellCoordinates>::iterator infectedPersonCoordinates = nullptr;
    while (infectorKey != keysOfInfectors.end()) {

        Person infector = infectedPopulationMap->value(*infectorKey);
        if (infector.vitalityState == VitalityState::healthy) {
            infectedPopulationMap->remove(*infectorKey);
            recoveredPopulationMap->insert(*infectorKey, infector);
            infectorKey = keysOfInfectors.erase(infectorKey);
            continue;
        }
        if (infector.vitalityState == VitalityState::dead) {
            infectorKey = keysOfInfectors.erase(infectorKey);
            continue;
        }

        QList<CellCoordinates> infectedPeopleCoordinates = findPeopleInfectedBy(&infector);
        for (infectedPersonCoordinates = infectedPeopleCoordinates.begin();
             infectedPersonCoordinates != infectedPeopleCoordinates.end();
             ++infectedPersonCoordinates) {
            if (!infectedPopulationMap->contains(*infectedPersonCoordinates) && !recoveredPopulationMap->contains(*infectedPersonCoordinates)) {
                Person infectedPerson;
                infectedPerson.calculateInfectionParameters(inputParameters);
                infectedPerson.updateVitalityState();
                infectedPopulationMap->insert(*infectedPersonCoordinates, infectedPerson);
                if (infectedPerson.stateIsChanged())
                    calculateOutputParameters(*infectedPersonCoordinates);
//            } else if (infectedPopulationMap->value(*infectedPersonCoordinates).vitalityState == VitalityState::healthy) {
            } else if (recoveredPopulationMap->contains(*infectedPersonCoordinates)) {
                //TODO: this case should be updated when the immunization starts to be considered during the simulation.
                /*
                Person infectedPerson;
                infectedPerson.calculateInfectionParameters(inputParameters);
                *infectedPopulationMap->insert(*infectedPersonCoordinates, infectedPerson);
                */
            }
        }

        infector.updateDayCounters();
        infectedPopulationMap->insert(*infectorKey, infector);
        if (infector.stateIsChanged())
            calculateOutputParameters(*infectorKey);

        // This row deletes the value where the iterator is and returns an iterator to the next element.
        infectorKey = keysOfInfectors.erase(infectorKey);
    }


    ++outputParameters.numberOfDays;
    populationStatusUpdated(infectedPopulationMap.data());
    updatedOutputParameters(&outputParameters);


}

QList<CellCoordinates> SimulationCore::findPeopleInfectedBy(const Person* infector)
{
//      And in the end if p = 1/9 for one person to be infected then for 3 days the expected number of infected people is 1*1/9 + 1*1/9 + 1*1/9 = 1/3.
//      Therefore p = 1/9 / 3 = 1/27 for three people to be infected for a day. For three days it is 3*1/27 + 3*1/27 + 3*1/27 = 1/3
    QList<CellCoordinates> infected;
    double maxInfectedPerDay = 3; // This number could also be randomly generated.

    double probabilityToInfect = infector->getProbabilityToInfect();
    probabilityToInfect /= maxInfectedPerDay;

    for (int i = 0; i < maxInfectedPerDay; ++i) {
        double randomInfectionKoeficient = randomGenerator.generateUniform(0, 1);
        if (randomInfectionKoeficient < probabilityToInfect) {
            int x = static_cast<int>(randomGenerator.generateUniform(0, peoplePerRow));
            int y = static_cast<int>(randomGenerator.generateUniform(0, peoplePerRow));
            infected.append(CellCoordinates(x, y));  // In this way there is a very little lower chance for the last of the row to appear.
        }
    }

    return infected;
}

void SimulationCore::updateMap()
{
/*------------------------------------------------------------------------------------
    InputPerameters parameters;
    infectedPopulationMap->insert(CellCoordinates(0,1), Person());
    (*infectedPopulationMap.data())[CellCoordinates(0,1)].getInfected(parameters);
    infectedPopulationMap->insert(CellCoordinates(100,100), Person());
    (*infectedPopulationMap.data())[CellCoordinates(100,100)].getInfected(parameters);
    infectedPopulationMap->insert(CellCoordinates(1,1), Person());
    (*infectedPopulationMap.data())[CellCoordinates(1,1)].getInfected(parameters);
    infectedPopulationMap->insert(CellCoordinates(200,129), Person());
    (*infectedPopulationMap.data())[CellCoordinates(200,129)].getInfected(parameters);

//    populationMap->drawNextGeneration(infectedPopulationMap.data());

    outputParameters.numberOfInfections = infectedPopulationMap->size();
    populationStatusUpdated(infectedPopulationMap.data());
    updatedOutputParameters(&outputParameters);
    -------------------------------------------------------------------------------------------*/

}

void SimulationCore::calculateOutputParameters(CellCoordinates coordinates)
{
    Person* clickedPerson = &(*infectedPopulationMap)[coordinates]; // If the key is not in the map it is automatically generated.

    VitalityState previousState = clickedPerson->previousVitalityState;
    VitalityState currentState = clickedPerson->vitalityState;

    switch (currentState) {
    case VitalityState::healthy:
    {
        if      (previousState == VitalityState::dead)
            --outputParameters.numberOfDeaths;
        else if (previousState == VitalityState::infected_severe_symptoms) {
            --outputParameters.numberOfSevereSymptoms;
            --outputParameters.numberOfInfections;
        }
        else if (previousState == VitalityState::infected_mild_symptoms) {
            --outputParameters.numberOfMildSymptoms;
            --outputParameters.numberOfInfections;
        }
        ++outputParameters.numberOfRecovered;
    }
        break;
    case VitalityState::infected_incubation:
        ++outputParameters.numberOfInfections;
        ++outputParameters.numberOfTotalInfections;
        break;
    case VitalityState::infected_mild_symptoms:
        ++outputParameters.numberOfMildSymptoms;
        break;
    case VitalityState::infected_severe_symptoms:
    {
        if (previousState == VitalityState::infected_mild_symptoms)
            --outputParameters.numberOfMildSymptoms;
        ++outputParameters.numberOfSevereSymptoms;
    }
        break;
    case VitalityState::dead:
    {
        --outputParameters.numberOfSevereSymptoms;
        --outputParameters.numberOfInfections;
        ++outputParameters.numberOfDeaths;
    }
        break;
    }

}

void SimulationCore::UpdateInputParameters(const InputPerameters* parameters)
{
    inputParameters = *parameters;
    qDebug() << inputParameters.deathRateMin;
}

OutputParameters SimulationCore::getOutputParameters()
{
    return outputParameters;

}

void SimulationCore::changeClickedPersonState(CellCoordinates cell)
{
    Person* clickedPerson = &(*infectedPopulationMap)[cell]; // If the key is not in the map it is automatically generated.
    switch (clickedPerson->vitalityState) {
    case VitalityState::healthy:
        ++outputParameters.numberOfInfections;
        ++outputParameters.numberOfTotalInfections;
        clickedPerson->calculateInfectionParameters(inputParameters);
        break;
    case VitalityState::infected_incubation:
        ++outputParameters.numberOfMildSymptoms;
        break;
    case VitalityState::infected_mild_symptoms:
        --outputParameters.numberOfMildSymptoms;
        ++outputParameters.numberOfSevereSymptoms;
        break;
    case VitalityState::infected_severe_symptoms:
        --outputParameters.numberOfInfections;
        --outputParameters.numberOfTotalInfections;
        --outputParameters.numberOfSevereSymptoms;
        ++outputParameters.numberOfDeaths;
        break;
    case VitalityState::dead:
        --outputParameters.numberOfDeaths;
        break;
    }

    clickedPerson->updateVitalityState(); //??????
    updatedOutputParameters(&outputParameters);
}

void SimulationCore::startSimulation()
{
    while (true) {
        updateInfectionsForTheDay();
        QThread::msleep(1000);
    }


}

void SimulationCore::update()
{
    updateInfectionsForTheDay();

}

