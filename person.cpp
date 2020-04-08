#include "globals.h"
#include "person.h"
#include "cmath"
#include "randomgenerator.h"

Person::Person()
{

}

void Person::updateDayCounters()
{
    switch (vitalityState) {
    case VitalityState::healthy:
        break;
    case VitalityState::infected_incubation:
        ++incubationDaysCounter;
        if (incubationDaysCounter == incubationPeriod) {
            if (isSevere)
                vitalityState = VitalityState::infected_severe_symptoms;
            else
                vitalityState = VitalityState::infected_mild_symptoms;
        }
        break;
    case VitalityState::infected_mild_symptoms:
        ++sicknessDaysCounter;
        if (sicknessDaysCounter > symptomsPeriod) {
            vitalityState = VitalityState::healthy;
        }
        break;
    case VitalityState::infected_severe_symptoms:
        ++sicknessDaysCounter;
        if (dayOfDeath == sicknessDaysCounter) {
            vitalityState = VitalityState::dead;
        }
        if (sicknessDaysCounter > symptomsPeriod) {
            vitalityState = VitalityState::healthy;
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

//    int dayOfDeath = 0;     // Day of the illness period
//    int incubationPeriod = 0;
//    int symptomsPeriod = 0;
//    double probabilityToInfect = 0;
//    bool isSevere = false;



//  Calculate incubation period.
    incubationPeriod = static_cast<int>(
                       Singleton::randomGenerator()
                       .generateNormal(parameters.incubationPeriodMean, parameters.incubationPeriodSigma));

//  Calculate period with symptoms.
    double severityOfTheInfection = Singleton::randomGenerator().generateUniform(0, 100);
    if (severityOfTheInfection <= parameters.persentSevereCases) {
        // Case is severe
        symptomsPeriod = static_cast<int>(Singleton::randomGenerator()
                         .generateNormal(parameters.severeSymptomsPeriodMean, parameters.severeSymptomsPeriodSigma));
        isSevere = true;

    } else {
        // Case is mild
        symptomsPeriod = static_cast<int>(Singleton::randomGenerator()
                         .generateNormal(parameters.mildSymptomsPeriodMean, parameters.mildSymptomsPeriodSigma));
        isSevere = false;
    }

//  Calculate death rate uniformly between min and max.
    double deathRate = Singleton::randomGenerator().generateUniform(parameters.deathRateMin, parameters.deathRateMax);
    double deathProbability = Singleton::randomGenerator().generateUniform(0,100);
    if (deathProbability < deathRate) {
        dayOfDeath = static_cast<int>(Singleton::randomGenerator().generateUniform(0, symptomsPeriod));
    }

//  Calculate probability to infect someone in a day.
    double transmisionRate = Singleton::randomGenerator().generateUniform(parameters.transmitionRateMin, parameters.transmisionRateMax);
    probabilityToInfect = transmisionRate/(incubationPeriod + symptomsPeriod);

    vitalityState = VitalityState::infected_incubation;
}

void Person::updateVitalityState()
{
    switch (vitalityState) {
    case VitalityState::healthy:
        vitalityState = VitalityState::infected_incubation;
        break;
    case VitalityState::infected_incubation:
        vitalityState = VitalityState::infected_mild_symptoms;
        break;
    case VitalityState::infected_mild_symptoms:
        vitalityState = VitalityState::infected_severe_symptoms;
        break;
    case VitalityState::infected_severe_symptoms:
        vitalityState = VitalityState::dead;
        break;
    case VitalityState::dead:
        vitalityState = VitalityState::healthy;
        break;
    }

}

double Person::getProbabilityToInfect() const
{
    return probabilityToInfect;
}
