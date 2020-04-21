#include "globals.h"
#include "person.h"
#include "cmath"
#include "randomgenerator.h"

Person::Person()
{

}

void Person::updateDayCounters(bool isThereVentilator)
{
    stateChanged = false;
    switch (vitalityState) {
    case VitalityState::healthy:
        break;
    case VitalityState::infected_incubation:
        ++incubationDaysCounter;
        if (incubationDaysCounter >= incubationPeriod) {
            if (isSevere) {
                updateVitalityStateTo(infected_severe_symptoms);
            }
            else {
                updateVitalityStateTo(infected_mild_symptoms);
            }
        }
        break;
    case VitalityState::infected_mild_symptoms:
        ++sicknessDaysCounter;
        if (sicknessDaysCounter > symptomsPeriod) {
            updateVitalityStateTo(healthy);
        }
        break;
    case VitalityState::infected_severe_symptoms:
        ++sicknessDaysCounter;
        if (sicknessDaysCounter >= dayOfICU && willBeInICU) {
            if (isThereVentilator) {
                updateVitalityStateTo(in_ICU);
            } else {
                updateVitalityStateTo(dead);
            }
        }
        if (sicknessDaysCounter > symptomsPeriod) {
            updateVitalityStateTo(healthy);
        }
        break;
    case VitalityState::in_ICU:
        ++sicknessDaysCounter;
        if (sicknessDaysCounter >= dayOfICU + dayOfDeathInICU && willDieInICU) {
            updateVitalityStateTo(dead);
        }
        else if (sicknessDaysCounter >= dayOfICU + ICUPeriod) {
            updateVitalityStateTo(infected_severe_symptoms);
        }
        break;
    case VitalityState::dead:
        break;
    }
}

void Person::updateProbabilityToInfect(const InputPerameters& parameters)
{
    double transmisionRate = Singleton::randomGenerator().generateUniform(parameters.transmissionRateMin, parameters.transmissionRateMax);
    probabilityToInfect = transmisionRate/(incubationPeriod + symptomsPeriod);
}

void Person::calculateInfectionParameters(const InputPerameters& parameters)
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
    if (isSevere) {
        double ICUProbability = Singleton::randomGenerator().generateUniform(0,100);
        if (ICUProbability <= parameters.percentSevereCasesForICU) {
            // Calculate day of getting into ICU and day of getting out. It will be if there is available ICU.
            ICUPeriod = static_cast<int>(Singleton::randomGenerator()
                                         .generateNormal(parameters.ICUPeriodMean, parameters.ICUPeriodSigma));
            dayOfICU = static_cast<int>(Singleton::randomGenerator().generateUniform(0, symptomsPeriod - ICUPeriod));
            willBeInICU = true;\

            // Calculate the will the person die in the ICU.
            double dieProbability = Singleton::randomGenerator().generateUniform(0,100);
            if (dieProbability <= parameters.ICUMortalityRate) {
                dayOfDeathInICU = static_cast<int>(Singleton::randomGenerator().generateUniform(0, ICUPeriod));
                willDieInICU = true;
            }
        }
    }

//  Calculate probability to infect someone in a day.
    double transmisionRate = Singleton::randomGenerator().generateUniform(parameters.transmissionRateMin, parameters.transmissionRateMax);
    probabilityToInfect = transmisionRate/(incubationPeriod + symptomsPeriod);

//    vitalityState = VitalityState::infected_incubation;
}

void Person::updateVitalityState()
{
    stateChanged = false;
    switch (vitalityState) {
    case VitalityState::healthy:
        updateVitalityStateTo(infected_incubation);
        break;
    case VitalityState::infected_incubation:
        updateVitalityStateTo(infected_mild_symptoms);
        break;
    case VitalityState::infected_mild_symptoms:
        updateVitalityStateTo(infected_severe_symptoms);
        break;
    case VitalityState::infected_severe_symptoms:
        updateVitalityStateTo(dead);
        break;
    case VitalityState::in_ICU:
        updateVitalityStateTo(dead);
        break;
    case VitalityState::dead:
        updateVitalityStateTo(healthy);
        break;
    }

}

void Person::updateVitalityStateTo(const VitalityState& newState)
{
    if (vitalityState != newState){
        previousVitalityState = vitalityState;
        vitalityState = newState;
        stateChanged = true;
    } else {
        stateChanged = false;
    }
}

double Person::getProbabilityToInfect() const
{
    return probabilityToInfect;
}

bool Person::stateIsChanged() const
{
    return stateChanged;
}
