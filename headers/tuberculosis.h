//
// Created by Vi1i on 11/22/16.
//

#ifndef MISIM_TUBERCULOSIS_H
#define MISIM_TUBERCULOSIS_H

#include <string>
#include <map>

class Tuberculosis {
public:
    enum Stage {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE
    };
    Tuberculosis(bool infected);
    ~Tuberculosis();


private:
    std::map<Stage, const std::string> stages = {
            {Stage::ZERO, "Not Infected"},
            {Stage::ONE, "Infected"},
            {Stage::TWO, "Latent Infection"},
            {Stage::THREE, "Active Infection"},
            {Stage::FOUR, "Cure"},
            {Stage::FIVE, "Death"}
    };
    
    std::string name;
    int current_stage;
};

#endif //MISIM_INFECTION_H
