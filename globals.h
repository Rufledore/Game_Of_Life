#ifndef GLOBALS_H
#define GLOBALS_H

#include <QHash>
#include <QPair>


enum VitalityState {healthy = 0, infected_incubation, infected_mild_symptoms, infected_severe_symptoms, in_ICU, dead};

struct InputPerameters
{

    // Input parameters
    double incubationPeriodMean = 0;
    double incubationPeriodSigma = 0;
    double mildSymptomsPeriodMean = 0;
    double mildSymptomsPeriodSigma = 0;
    double severeSymptomsPeriodMean = 0;
    double severeSymptomsPeriodSigma = 0;
    double ICUPeriodMean = 0;
    double ICUPeriodSigma = 0;
    double transmissionRateMin = 0;
    double transmissionRateMax = 0;
    double persentSevereCases = 0;
    double ventilatorsPerCapita = 0;
    double ICUMortalityRate = 0;
    double percentSevereCasesForICU = 0;
};

struct OutputParameters
{

    // Output parameters
    int numberOfBusyVentilators = 0;
    int numberOfVentilators = 0;
    int numberOfTotalInfections = 0;
    int numberOfInfections = 0;
    int numberOfMildSymptoms = 0;
    int numberOfSevereSymptoms = 0;
    int numberOfDeaths = 0;
    int numberOfRecovered = 0;
    int numberOfDays = 0;
};



struct Constants
{
    static const int numberOfCellsPerRow;

};


#endif // GLOBALS_H
