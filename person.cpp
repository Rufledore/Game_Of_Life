#include "globals.h"
#include "person.h"
#include "cmath"
#include "randomgenerator.h"

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

void Person::getInfected(const InputPerameters& parameters)
{
//  This methods calculates all the necessary variable that a person needs when he is infected.

//  Calculate incubation period.
    incubationPeriod = static_cast<int>(
                       Singleton::randomGenerator()
                       .generateNormal(parameters.incubationPeriodMean, parameters.incubationPeriodSigma));

//  Calculate period with symptoms.
    double severityOfTheInfection = Singleton::randomGenerator().generateUniform(0, 100);
    if (severityOfTheInfection <= parameters.sereveCasesPercent) {
        // Case is severe
        symptomsPeriod = Singleton::randomGenerator()
                         .generateUniform(parameters.severeSymptomsPeriodMean, parameters.severeSymptomsPeriodSigma);
    }

//  Calculate death rate uniformly between min and max.
    double deathRate = Singleton::randomGenerator().generateUniform(parameters.deathRateMin, parameters.deathRateMax);
    double deathProbability = Singleton::randomGenerator().generateUniform(0,100);
    if (deathProbability < deathRate) {
        dayOfDeath = static_cast<int>(QRandomGenerator::global()->generateDouble() * sicknessPeriod);
    }


    vitalityState = VitalityState::infected_incubation;
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

double Person::getProbabilityToInfect() const
{
    return probabilityToInfect;
}
