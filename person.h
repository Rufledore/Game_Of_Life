#ifndef PERSON_H
#define PERSON_H

#include "globals.h"
#include "randomgenerator.h"

class Person
{
public:
    Person();
    void updateDayCounters(int incubationPeriod, int illnessPeriod);
    void getInfected(const InputPerameters& parameters, const RandomGenerator& generator);
    void updateVitalityState();


    int incubationDaysCounter = 0;
    int sicknessDaysCounter = 0;
    VitalityState vitalityState = VitalityState::healty;


    double getProbabilityToInfect() const;

private:
    int dayOfDeath = 0;
    double probabilityToInfect = 0;

};

typedef QPair<int, int> CellCoordinates; // QHash doesn't work with QPoint
typedef QHash<CellCoordinates, Person> InfectionMap;
#endif // PERSON_H
