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
    Tuberculosis(bool infected, bool vaccinated);
    ~Tuberculosis();

    void NextStage(double chance);
    std::string GetStage(int stage);
    bool Infected();
    bool CanInfect();
    void NewDay();
    void Vaccinate();
    bool Vaccinated();
    bool Dead();
private:
    std::map<Stage, const std::string> _stages = {
            {Stage::ZERO, "Not Infected"},
            {Stage::ONE, "Infected"},
            {Stage::TWO, "Latent Infection"},
            {Stage::THREE, "Active Infection"},
            {Stage::FOUR, "Cure"},
            {Stage::FIVE, "Death"}
    };
    
    std::string _name;
    int _current_stage;
    bool _infected;
    bool _vaccinated;
    int _days_since_infection;
};

#endif //MISIM_INFECTION_H
