#include <random>

#include "randomgenerator.h"

RandomGenerator::RandomGenerator()
{

}

RandomGenerator::~RandomGenerator()
{

}

double RandomGenerator::generateUniform(double min, double max)
{
    return uniformGenerator.generateDouble() * (max - min) + min;
}

double RandomGenerator::generateNormal(double mean, double stddev)
{
    normalGenerator = std::normal_distribution<double>(mean, stddev);
    return normalGenerator(uniformGenerator);
}

RandomGenerator RandomGenerator::global()
{
    return RandomGenerator();
}
