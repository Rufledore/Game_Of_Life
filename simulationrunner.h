#ifndef SIMULATIONRUNNER_H
#define SIMULATIONRUNNER_H

#include <QObject>
#include <QSharedPointer>

#include <person.h>

struct InputPerameters {

    // Input parameters
    double incubationPeriodMin = 0;
    double incubationPeriodMax = 0;
    double illnessPeriodMin = 0;
    double illnessPeriodMax = 0;
    double infectionRateMin = 0;
    double infectionRateMax = 0;
    double deathRateMin = 0;
    double deathRateMax = 0;
};

struct OutputParameters {

    // Output parameters
    int numberOfInfections = 0;
    int numberOfDays = 0;
    int numberOfDeaths = 0;
};

class SimulationRunner : public QObject
{
    Q_OBJECT
public:
    explicit SimulationRunner(QObject *parent = nullptr);



    // Map of infected people.
    QSharedPointer<InfectionMap> infectedPopulationMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.

    void updateMap();
    void UpdateInputParameters(InputPerameters* parameters);
    OutputParameters getOutputParameters();

signals:
    void populationStatusUpdated(InfectionMap* map);
    void updatedOutputParameters(OutputParameters* parameters);

public slots:
    void changeClickedPersonState(CellCoordinates cell);

private:
    InputPerameters inputParameters;
    OutputParameters outputParameters;

private slots:
};

#endif // SIMULATIONRUNNER_H
