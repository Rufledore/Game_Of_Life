#ifndef SIMULATIONRUNNER_H
#define SIMULATIONRUNNER_H

#include <QObject>
#include <QSharedPointer>

#include <person.h>

struct InputPerameters {

    // Input parameters
    double m_incubationPeriod = 0;
    double m_illnessPeriod = 0;
    double m_infectionRate = 0;
    double m_deathRate = 0;
};

struct OutputParameters {

    // Output parameters
    int m_numberOfInfected = 0;
    int m_numberOfDays = 0;
};

class SimulationRunner : public QObject
{
    Q_OBJECT
public:
    explicit SimulationRunner(QObject *parent = nullptr);



    // Map of infected people.
    QSharedPointer<InfectionMap> infectedPopulationMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.

    void updateMap();

signals:
    void populationStatusUpdated(InfectionMap* map);
    void updatedOutputParameters(OutputParameters* parameters);

private:
    InputPerameters inputParameters;
    OutputParameters outputParameters;

private slots:
    void UpdateInputParameters(InputPerameters* parameters);
};

#endif // SIMULATIONRUNNER_H
