//
// Created by Vi1i on 11/22/16.
//

#ifndef MISIM_INFECTION_H
#define MISIM_INFECTION_H

class Infection {
public:
    Infection();
    ~Infection();

    void Init();

private:
    std::string name;
    float mod;
    int stage;
};

#endif //MISIM_INFECTION_H
