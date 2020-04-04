#ifndef PERSON_H
#define PERSON_H

#include "globals.h"


class Person
{
public:
    Person();
    void updateDayCounters(int incubationPeriod, int illnessPeriod);
    void infect(int sicknessPeriod, double deathRate);
    void updateVitalityState();

    int incubationDaysCounter = 0;
    int sicknessDaysCounter = 0;
    VitalityState vitalityState = VitalityState::healty;


private:
    int dayOfDeath = 0;

};

typedef QPair<int, int> CellCoordinates; // QHash doesn't work with QPoint
typedef QHash<CellCoordinates, Person> InfectionMap;
#endif // PERSON_H
