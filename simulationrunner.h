#ifndef SIMULATIONRUNNER_H
#define SIMULATIONRUNNER_H

#include <QObject>
#include <QSharedPointer>

#include <person.h>
#include <randomgenerator.h>


class SimulationRunner : public QObject
{
    Q_OBJECT
public:
    explicit SimulationRunner(QObject *parent = nullptr);

    // Map of infected people.
    QSharedPointer<InfectionMap> infectedPopulationMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QList<CellCoordinates> keysOfInfectors;

    // Simulation methods:
    void updateInfectionsForTheDay();
    QList<CellCoordinates> findPeopleInfectedBy(const Person* infector);

    void updateMap();
    void UpdateInputParameters(InputPerameters* parameters);


    // Getters
    OutputParameters getOutputParameters();

signals:
    void populationStatusUpdated(const InfectionMap* map);
    void updatedOutputParameters(OutputParameters* parameters);

public slots:
    void changeClickedPersonState(CellCoordinates cell);

private:
    InputPerameters inputParameters;
    OutputParameters outputParameters;
    RandomGenerator randomGenerator;

    int peoplePerRow = Constants::numberOfCellsPerRow;

private slots:
};

#endif // SIMULATIONRUNNER_H
