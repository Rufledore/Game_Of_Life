#include <QTime>

#include <random>
#include <cmath>

#include "randomgenerator.h"

RandomGenerator::RandomGenerator()
{

}

RandomGenerator::~RandomGenerator()
{

}

double RandomGenerator::generateUniform(double min, double max)
{
    if (max < min) {
    //  Max can't be smaller than Min
        double i = min;
        min = max;
        max = i;
    }

//    int seed = QTime::currentTime().msecsSinceStartOfDay();
//    QRandomGenerator generator = QRandomGenerator(seed);

    return uniformGenerator.generateDouble() * std::abs(max - min) + min;
}

double RandomGenerator::generateNormal(double mean, double stddev)
{
//    int seed = QTime::currentTime().msecsSinceStartOfDay();
//    QRandomGenerator generator = QRandomGenerator(seed);
    normalGenerator = std::normal_distribution<double>(mean, stddev);
    return normalGenerator(uniformGenerator);
}
