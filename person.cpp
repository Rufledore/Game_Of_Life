#include <QRandomGenerator>

#include "person.h"
#include "cmath"

Person::Person()
{

}

void Person::updateDayCounters(int incubationPeriod, int sicknessPeriod)
{
    if (isAlive && isInfected) {
        if (inIncubation) {
            ++incubationDaysCounter;
            if (incubationDaysCounter == incubationPeriod) {
                inIncubation = false;
            }
        } else {        // Is sick
            ++sicknessDaysCounter;
            if (willDie && dayOfDeath == sicknessDaysCounter) {
                isAlive = false;
            }
            if (sicknessDaysCounter > sicknessPeriod) {
                isInfected = false;
                inIncubation = false;
            }
        }
    }
}

void Person::infect(int sicknessPeriod, double deathRate)
{
    double randomDeath = QRandomGenerator::global()->generateDouble() * 100;

    isInfected = true;
    if (randomDeath < deathRate) {
        willDie = true;
        dayOfDeath = static_cast<int>(QRandomGenerator::global()->generateDouble() * sicknessPeriod);
    }
}
