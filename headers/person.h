//
// Created by Vi1i on 11/22/16.
//

#ifndef MISIM_PERSON_H
#define MISIM_PERSON_H

class Person {
public:
    Person();
    ~Person();
private:
    unsigned int _id;
    bool infected;
    bool latentInfection;
    double chanceToBeInfected;
    double chanceToInfect;
};

#endif //MISIM_PERSON_H