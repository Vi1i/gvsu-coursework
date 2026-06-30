//
// Created by Vi1i on 11/22/16.
//

#include "../headers/Person.h"

Person::Person(Person * person) {
    *this = person;
}

Person::Person(unsigned int id, Tuberculosis * tb) {
    this->_id = id;
    this->_tb = tb;
}

Person::~Person() {
}

void Person::Step(int cur_day, double chance) {
    if(this->_last_day < cur_day) {
        this->_tb->NewDay();
        this->_last_day = cur_day;
    }
    this->_tb->NextStage(chance);
}

void Person::Vaccinate() {
    this->_tb->Vaccinate();
}

bool Person::CanInfect() {
    return this->_tb->CanInfect();
}

bool Person::Vaccinated() {
    return this->_tb->Vaccinated();
}

bool Person::Dead() {
    return this->_tb->Dead();
}

bool Person::Infected() {
    return this->_tb->Infected();
}

unsigned int Person::ID() {
    return this->_id;
}

bool Person::operator==(const Person &p2) {
    return this->_id == p2._id;
}

bool Person::operator!=(const Person &p2) {
    return this->_id != p2._id;
}

bool Person::operator<(const Person &p2) {
    return this->_id < p2._id;
}

bool Person::operator<=(const Person &p2) {
    return this->_id <= p2._id;
}

bool Person::operator>(const Person &p2) {
    return this->_id > p2._id;
}

bool Person::operator>=(const Person &p2) {
    return this->_id >= p2._id;
}