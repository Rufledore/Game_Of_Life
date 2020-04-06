#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>
#include <QRandomGenerator>


class RandomGenerator
{

public:
    RandomGenerator();
    ~RandomGenerator();
    double generateNormal(double mean, double stddev);
    double generateUniform(double min, double max);

private:
    QRandomGenerator uniformGenerator = QRandomGenerator(42);
    std::normal_distribution<double> normalGenerator;
};

struct Singleton
{
    static RandomGenerator& randomGenerator()
    {
        static RandomGenerator generator;
        return generator;
    }
};

#endif // RANDOMGENERATOR_H
