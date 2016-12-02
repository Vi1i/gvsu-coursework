//
// Created by Vi1i on 11/28/16.
//

#include "../headers/Tuberculosis.h"

Tuberculosis::Tuberculosis(bool infected) {
    this->_infected = infected;
    this->_name = "Tuberculosis";
    this->_current_stage = Stage::ZERO;
}