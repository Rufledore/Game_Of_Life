#include <QRandomGenerator>

#include "person.h"
#include "cmath"

Person::Person()
{

}

void Person::updateDayCounters(int incubationPeriod, int sicknessPeriod)
{
    switch (vitalityState) {
    case VitalityState::healty:
        break;
    case VitalityState::infected_incubation:
        ++incubationDaysCounter;
        if (incubationDaysCounter == incubationPeriod) {
            vitalityState = VitalityState::infected_sick;
        }
        break;
    case VitalityState::infected_sick:
        ++sicknessDaysCounter;
        if (dayOfDeath == sicknessDaysCounter) {
            vitalityState = VitalityState::dead;
        }
        if (sicknessDaysCounter > sicknessPeriod) {
            vitalityState = VitalityState::healty;
        }
        break;
    case VitalityState::dead:
        break;
    }
//    if (isAlive && isInfected) {
//        if (inIncubation) {
//            ++incubationDaysCounter;
//            if (incubationDaysCounter == incubationPeriod) {
//                inIncubation = false;
//            }
//        } else {        // Is sick
//            ++sicknessDaysCounter;
//            if (dayOfDeath == sicknessDaysCounter) {
//                isAlive = false;
//            }
//            if (sicknessDaysCounter > sicknessPeriod) {
//                isInfected = false;
//                inIncubation = false;
//            }
//        }
//    }
}

void Person::infect(int sicknessPeriod, double deathRate)
{
    double deathProbability = QRandomGenerator::global()->generateDouble() * 100;

    vitalityState = VitalityState::infected_incubation;
    if (deathProbability < deathRate) {
        dayOfDeath = static_cast<int>(QRandomGenerator::global()->generateDouble() * sicknessPeriod);
    }
}

void Person::updateVitalityState()
{
    switch (vitalityState) {
    case VitalityState::healty:
        vitalityState = VitalityState::infected_incubation;
        break;
    case VitalityState::infected_incubation:
        vitalityState = VitalityState::infected_sick;
        break;
    case VitalityState::infected_sick:
        vitalityState = VitalityState::dead;
        break;
    case VitalityState::dead:
        vitalityState = VitalityState::healty;
        break;
    }

}
