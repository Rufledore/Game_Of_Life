#ifndef PERSON_H
#define PERSON_H

#include "globals.h"
#include "randomgenerator.h"

class Person
{
public:
    Person();
    void updateDayCounters();
    void updateProbabilityToInfect(const InputPerameters& parameters);
    void calculateInfectionParameters(const InputPerameters& parameters);
    void updateVitalityState();
    void updateVitalityStateTo(const VitalityState& newState);

    VitalityState previousVitalityState = VitalityState::healthy;
    VitalityState vitalityState         = VitalityState::healthy;

    double getProbabilityToInfect() const;

    bool stateIsChanged() const;

private:
    int dayOfDeath = 0;     // Day of the illness period
    int incubationPeriod = 0;
    int symptomsPeriod = 0;
    double probabilityToInfect = 0;
    bool isSevere = false;
    bool stateChanged = false;
    bool needsICU = false;

    int incubationDaysCounter = 0;
    int sicknessDaysCounter = 0;
};

typedef QPair<int, int> CellCoordinates; // QHash doesn't work with QPoint
typedef QHash<CellCoordinates, Person> InfectionMap;
#endif // PERSON_H
