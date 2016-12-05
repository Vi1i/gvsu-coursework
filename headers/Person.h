//
// Created by Vi1i on 11/22/16.
//

#ifndef MISIM_PERSON_H
#define MISIM_PERSON_H

#include "Tuberculosis.h"

class Person {
public:
    Person(Person * person);
    Person(unsigned int id, Tuberculosis * tb);
    ~Person();

    void Step(int cur_day, double chance);
    bool Infected();
    void Vaccinate();
    bool Vaccinated();
    bool CanInfect();
    bool Dead();
    unsigned int ID();

    bool operator == (const Person &p2);
    bool operator != (const Person &p2);
    bool operator > (const Person &p2);
    bool operator < (const Person &p2);
    bool operator <= (const Person &p2);
    bool operator >= (const Person &p2);

private:

    unsigned int _id;
    Tuberculosis * _tb;
    int _last_day;
};

#endif //MISIM_PERSON_H
