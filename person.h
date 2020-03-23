#ifndef PERSON_H
#define PERSON_H


class Person
{
public:
    Person();
    void updateDayCounters(int incubationPeriod, int illnessPeriod);
    void infect(int sicknessPeriod, double deathRate);

    bool isInfected = false;
    bool isAlive = true;
    bool inIncubation = false;
    bool isSick = false;
    int incubationDaysCounter = 0;
    int sicknessDaysCounter = 0;


private:
    int dayOfDeath = 0;

};

#endif // PERSON_H
