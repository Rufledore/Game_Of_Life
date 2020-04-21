#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#include "simulationcore.h"
#include "simulationrunner.h"

SimulationCore::SimulationCore(QObject *parent, int peoplePerRow) :
    QObject(parent),
    infectedPopulationMap(new InfectionMap),
    recoveredPopulationMap(new InfectionMap),
    m_peoplePerRow(peoplePerRow)
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
                infectedPerson.calculateInfectionParameters(m_inputParameters);
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

        bool thereIsVentilator = m_outputParameters.numberOfVentilators - m_outputParameters.numberOfBusyVentilators > 0;
        infector.updateDayCounters(thereIsVentilator);
        infectedPopulationMap->insert(*infectorKey, infector);
        if (infector.stateIsChanged())
            calculateOutputParameters(*infectorKey);

        // This row deletes the value where the iterator is and returns an iterator to the next element.
        infectorKey = keysOfInfectors.erase(infectorKey);
    }


    ++m_outputParameters.numberOfDays;
    populationStatusUpdated(infectedPopulationMap.data());
    updatedOutputParameters(&m_outputParameters);


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
            int x = static_cast<int>(randomGenerator.generateUniform(0, m_peoplePerRow));
            int y = static_cast<int>(randomGenerator.generateUniform(0, m_peoplePerRow));
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

void SimulationCore::restart(int peoplePerRow)
{
    m_isRunning = false;
    m_peoplePerRow = peoplePerRow;


    infectedPopulationMap->clear();
    recoveredPopulationMap->clear();
    m_outputParameters.numberOfDays = 0;
    m_outputParameters.numberOfDeaths = 0;
    m_outputParameters.numberOfRecovered = 0;
    m_outputParameters.numberOfInfections = 0;
    m_outputParameters.numberOfMildSymptoms = 0;
    m_outputParameters.numberOfSevereSymptoms = 0;
    m_outputParameters.numberOfTotalInfections = 0;
    m_outputParameters.numberOfBusyVentilators = 0;
    m_outputParameters.numberOfVentilators = static_cast<int>(m_peoplePerRow
                                                              * m_peoplePerRow
                                                              * m_inputParameters.ventilatorsPerCapita);

    updatedOutputParameters(&m_outputParameters);
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
            --m_outputParameters.numberOfDeaths;
        else if (previousState == VitalityState::infected_severe_symptoms) {
            --m_outputParameters.numberOfSevereSymptoms;
            --m_outputParameters.numberOfInfections;
        }
        else if (previousState == VitalityState::infected_mild_symptoms) {
            --m_outputParameters.numberOfMildSymptoms;
            --m_outputParameters.numberOfInfections;
        }
        ++m_outputParameters.numberOfRecovered;
    }
        break;
    case VitalityState::infected_incubation:
        ++m_outputParameters.numberOfInfections;
        ++m_outputParameters.numberOfTotalInfections;
        break;
    case VitalityState::infected_mild_symptoms:
        ++m_outputParameters.numberOfMildSymptoms;
        break;
    case VitalityState::infected_severe_symptoms:
    {
        if (previousState == VitalityState::infected_mild_symptoms)
            --m_outputParameters.numberOfMildSymptoms;
        if (previousState == VitalityState::in_ICU)
            --m_outputParameters.numberOfBusyVentilators;
        ++m_outputParameters.numberOfSevereSymptoms;
    }
        break;
    case VitalityState::in_ICU:
        ++m_outputParameters.numberOfBusyVentilators;
        break;
    case VitalityState::dead:
    {
        --m_outputParameters.numberOfSevereSymptoms;
        --m_outputParameters.numberOfInfections;
        ++m_outputParameters.numberOfDeaths;
    }
        break;
    }

}

void SimulationCore::UpdateInputParameters(const InputPerameters* parameters)
{
    m_inputParameters = *parameters;

    updateNumberOfVentilators();
}

void SimulationCore::UpdateTransmissionProbability(double min, double max)
{
    m_inputParameters.transmissionRateMin = min;
    m_inputParameters.transmissionRateMax = max;

    keysOfInfectors.clear();
    keysOfInfectors = infectedPopulationMap->keys();
    QList<CellCoordinates>::iterator infectorKey = keysOfInfectors.begin();

    while (infectorKey != keysOfInfectors.end()) {
        (*infectedPopulationMap)[*infectorKey].updateProbabilityToInfect(m_inputParameters);
        infectorKey = keysOfInfectors.erase(infectorKey);
    }

}

OutputParameters SimulationCore::getOutputParameters()
{
    return m_outputParameters;

}

void SimulationCore::changeClickedPersonState(CellCoordinates cell)
{
    Person* clickedPerson = &(*infectedPopulationMap)[cell]; // If the key is not in the map it is automatically generated.
    switch (clickedPerson->vitalityState) {
    case VitalityState::healthy:
        ++m_outputParameters.numberOfInfections;
        ++m_outputParameters.numberOfTotalInfections;
        clickedPerson->calculateInfectionParameters(m_inputParameters);
        break;
    case VitalityState::infected_incubation:
        ++m_outputParameters.numberOfMildSymptoms;
        break;
    case VitalityState::infected_mild_symptoms:
        --m_outputParameters.numberOfMildSymptoms;
        ++m_outputParameters.numberOfSevereSymptoms;
        break;
    case VitalityState::infected_severe_symptoms:
        --m_outputParameters.numberOfInfections;
        --m_outputParameters.numberOfTotalInfections;
        --m_outputParameters.numberOfSevereSymptoms;
        ++m_outputParameters.numberOfDeaths;
        break;
    case VitalityState::in_ICU:
        --m_outputParameters.numberOfInfections;
        --m_outputParameters.numberOfTotalInfections;
        --m_outputParameters.numberOfSevereSymptoms;
        ++m_outputParameters.numberOfDeaths;
        break;
    case VitalityState::dead:
        --m_outputParameters.numberOfDeaths;
        break;
    }

    clickedPerson->updateVitalityState(); //??????

    if (!m_isRunning)
        updatedOutputParameters(&m_outputParameters);
}

void SimulationCore::update()
{
    updateInfectionsForTheDay();

}

void SimulationCore::start()
{
    if (m_isRunning)
        return;

    m_isRunning = true;

    QScopedPointer<SimulationRunner> runner(new SimulationRunner);
    connect(runner.data(), &SimulationRunner::finished, runner.data(), &QObject::deleteLater);
    runner->startSimulation(this);
}

void SimulationCore::stop()
{
    m_isRunning = false;
}

void SimulationCore::updateNumberOfVentilators()
{
    m_outputParameters.numberOfVentilators = static_cast<int>(m_inputParameters.ventilatorsPerCapita * m_peoplePerRow * m_peoplePerRow);
    updatedOutputParameters(&m_outputParameters);
}

void SimulationCore::runningSimulation()
{
    int frameTime = 200;
    int baseTime = QTime::currentTime().msecsSinceStartOfDay();
    while (m_isRunning && m_outputParameters.numberOfInfections != 0) {
        int currentTime = QTime::currentTime().msecsSinceStartOfDay();

        if (currentTime - baseTime > frameTime) {
            m_simulationMutex.lock();
            updateInfectionsForTheDay();
            m_simulationMutex.unlock();
            baseTime = currentTime;
        }
        QThread::msleep(20);

        QCoreApplication::processEvents();
    }
}
