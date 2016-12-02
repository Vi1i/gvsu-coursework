//
// Created by Vi1i on 11/22/16.
//

#ifndef MISIM_PERSON_H
#define MISIM_PERSON_H

#include "tuberculosis.h"
#include "school.h"

class Person {
public:
    Person(unsigned int id, Tuberculosis * tb, School * school);
    ~Person();
private:
    unsigned int _id;
    Tuberculosis * _tb;
    School * _school;
};

#endif //MISIM_PERSON_H
