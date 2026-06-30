//
// Created by Vi1i on 11/28/16.
//

#include "../headers/Tuberculosis.h"

Tuberculosis::Tuberculosis(bool infected, bool vaccinated) {
    this->_infected = infected;
    this->_vaccinated = vaccinated;
    this->_name = "Tuberculosis";
    if(this->_infected) {
        this->_current_stage = Stage::ONE;
    }else {
        this->_current_stage = Stage::ZERO;
    }
    this->_days_since_infection = 0;
}

Tuberculosis::~Tuberculosis() {

}

void Tuberculosis::NextStage(double chance) {
    switch(this->_current_stage) {
        case Tuberculosis::ZERO : // Not infected
            if(chance >= .95) {
                this->_current_stage = Tuberculosis::ONE;
                this->_infected = true;
            }
            break;
        case Tuberculosis::ONE : // Infected
            if(this->_days_since_infection >= 28) {
                if (chance < .95) { // Latent Infection
                    this->_current_stage = Tuberculosis::TWO;
                } else { // Active infection
                    this->_current_stage = Tuberculosis::THREE;
                }
            }
            break;
        case Tuberculosis::TWO : // Latent infection
            if(chance >= .95) { // Becomes active
                this->_current_stage = Tuberculosis::THREE;
            }
            break;
        case Tuberculosis::THREE : // Active infection
            if(this->_vaccinated) {
                if(chance >= .95) {
                    this->_current_stage = Tuberculosis::FOUR;
                }else{
                    this->_current_stage = Tuberculosis::FIVE;
                }
            }else{
                if(chance >= .5) {
                    this->_current_stage = Tuberculosis::FOUR;
                }else{
                    this->_current_stage = Tuberculosis::FIVE;
                }
            }
            break;
        case Tuberculosis::FOUR : // Cure
            this->_infected = false;
            if(chance >=.95) {
                this->_infected = true;
                this->_current_stage = Tuberculosis::THREE;
            }
            break;
        case Tuberculosis::FIVE : // Death
            this->_infected = false;
            break;
        default :
            break;
    }
}

std::string Tuberculosis::GetStage(int stage) {
    return this->_stages.at((Tuberculosis::Stage)stage);
}

bool Tuberculosis::Infected() {
    return this->_infected;
}

bool Tuberculosis::CanInfect() {
    return this->_current_stage == Tuberculosis::THREE;
}

void Tuberculosis::NewDay() {
    if(this->_infected) {
        this->_days_since_infection++;
    }
}

bool Tuberculosis::Vaccinated() {
    return this->_vaccinated;
}

void Tuberculosis::Vaccinate() {
    this->_vaccinated = true;
}

bool Tuberculosis::Dead() {
    return this->_current_stage == Tuberculosis::FIVE;
}