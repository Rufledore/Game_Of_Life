#ifndef GLOBALS_H
#define GLOBALS_H

#include <QHash>
#include <QPair>


enum VitalityState {healthy = 0, infected_incubation, infected_mild_symptoms, infected_severe_symptoms, dead};

struct InputPerameters
{

    // Input parameters
    double incubationPeriodMean = 0;
    double incubationPeriodSigma = 0;
    double mildSymptomsPeriodMean = 0;
    double mildSymptomsPeriodSigma = 0;
    double severeSymptomsPeriodMean = 0;
    double severeSymptomsPeriodSigma = 0;
    double transmitionRateMin = 0;
    double transmisionRateMax = 0;
    double deathRateMin = 0;
    double deathRateMax = 0;
    double persentSevereCases = 0;
};

struct OutputParameters
{

    // Output parameters
    int numberOfInfections = 0;
    int numberOfMildSymptoms = 0;
    int numberOfSevereSymptoms = 0;
    int numberOfDeaths = 0;
    int numberOfDays = 0;
};



struct Constants
{
    static const int numberOfCellsPerRow;

};


#endif // GLOBALS_H
