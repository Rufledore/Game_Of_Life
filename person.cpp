#include <QRandomGenerator>

#include "person.h"
#include "cmath"

Person::Person()
{

}

void Person::updateDayCounters(int incubationPeriod, int sicknessPeriod)
{
    switch (vitalityState) {
    case Globals::VitalityState::healty:
        break;
    case Globals::VitalityState::infected_incubation:
        ++incubationDaysCounter;
        if (incubationDaysCounter == incubationPeriod) {
            vitalityState = Globals::VitalityState::infected_sick;
        }
        break;
    case Globals::VitalityState::infected_sick:
        ++sicknessDaysCounter;
        if (dayOfDeath == sicknessDaysCounter) {
            vitalityState = Globals::VitalityState::dead;
        }
        if (sicknessDaysCounter > sicknessPeriod) {
            vitalityState = Globals::VitalityState::healty;
        }
        break;
    case Globals::VitalityState::dead:
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

    vitalityState = Globals::VitalityState::infected_incubation;
    if (deathProbability < deathRate) {
        dayOfDeath = static_cast<int>(QRandomGenerator::global()->generateDouble() * sicknessPeriod);
    }
}
