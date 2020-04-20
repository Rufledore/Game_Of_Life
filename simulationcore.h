#ifndef SIMULATIONCORE_H
#define SIMULATIONCORE_H

#include <QObject>
#include <QSharedPointer>
#include <QMutex>

#include <person.h>
#include <randomgenerator.h>


class SimulationCore : public QObject
{
    Q_OBJECT
public:
    explicit SimulationCore(QObject *parent = nullptr, int peoplePerRow = Constants::numberOfCellsPerRow);

    // Multi-Threading parameters
    bool m_isRunning = false;
    QMutex m_simulationMutex;

    // Map of infected people.
    QSharedPointer<InfectionMap> infectedPopulationMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QSharedPointer<InfectionMap> recoveredPopulationMap;              //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QList<CellCoordinates> keysOfInfectors;

    // Simulation methods:
    void runningSimulation();
    void updateInfectionsForTheDay();
    QList<CellCoordinates> findPeopleInfectedBy(const Person* infector);

    void updateMap();
    void restart(int peoplePerRow);
    void calculateOutputParameters(CellCoordinates coordinates);
    void UpdateInputParameters(const InputPerameters* parameters);
    void UpdateTransmissionProbability(double min, double max);

    // Getters
    OutputParameters getOutputParameters();

signals:
    void populationStatusUpdated(const InfectionMap* map);
    void updatedOutputParameters(OutputParameters* parameters);

public slots:
    void changeClickedPersonState(CellCoordinates cell);
    void update();
    void start();
    void stop();

private:
    InputPerameters m_inputParameters;
    OutputParameters m_outputParameters;
    RandomGenerator randomGenerator;
    int m_peoplePerRow;

    void updateNumberOfVentilators();
};

#endif // SIMULATIONCORE_H
