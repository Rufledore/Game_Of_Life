#ifndef GLOBALS_H
#define GLOBALS_H

#include <QHash>
#include <QPair>


enum VitalityState {healty = 0, infected_incubation, infected_sick, dead};

struct InputPerameters
{

    // Input parameters
    double incubationPeriodMean = 0;
    double incubationPeriodSigma = 0;
    double mildSymptomsPeriodMean = 0;
    double mildSymptomsPeriodSigma = 0;
    double severeSymptomsPeriodMean = 0;
    double severeSymptomsPeriodSigma = 0;
    double infectionRateMin = 0;
    double infectionRateMax = 0;
    double deathRateMin = 0;
    double deathRateMax = 0;
    double sereveCasesPercent = 0;
};

struct OutputParameters
{

    // Output parameters
    int numberOfInfections = 0;
    int numberOfDays = 0;
    int numberOfDeaths = 0;
};



struct Constants
{
    static const int numberOfCellsPerRow;

};


#endif // GLOBALS_H
