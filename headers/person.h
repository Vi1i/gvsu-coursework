//
// Created by Vi1i on 11/22/16.
//

#ifndef MISIM_PERSON_H
#define MISIM_PERSON_H

#include "tuberculosis.h"

class Person {
public:
    Person(bool infected);
    ~Person();
private:
    unsigned int _county_id;
    unsigned int _school_id;
    unsigned int _id;
    Tuberculosis _tb;
};

#endif //MISIM_PERSON_H
