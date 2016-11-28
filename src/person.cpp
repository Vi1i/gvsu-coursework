//
// Created by Vi1i on 11/22/16.
//

#include "../headers/person.h"

Person::Person(unsigned int id, Tuberculosis tb, School school) {
    this->_id = id;
    this->_tb = tb;
    this->_school = school;
}
Person::~Person() {

}