//
// Created by Vi1i on 11/28/16.
//

#include "../headers/Sim.h"

Sim::Sim(std::string filename_in) {
    this->filename_in = filename_in;
    this->filename_out = "out-" + this->filename_in;
}

Sim::Sim(std::string filename_in, std::string filename_out) {
    this->filename_in = filename_in;
    this->filename_out = filename_out;
}

void Sim::Run() {

}

bool Sim::Init() {

}

bool Sim::Read() {

}

bool Sim::Write() {

}

void Sim::Step() {

}