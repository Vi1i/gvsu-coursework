//
// Created by Lawrence L. O'Boyle II on 11/28/16.
//

#ifndef MISIM_SCHOOL_H
#define MISIM_SCHOOL_H

#include <string>
#include <map>

class School {
public:
    enum Grade {
        K,
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
        FIFTH,
        SIXTH,
        SEVENTH,
        EIGTH,
        NINTH,
        TENTH,
        ELEVENTH,
        TWELTH
    };

    School();
    ~School();
private:
    std::map<Grade, const std::string> grades = {
            {Grade::K, "K"},
            {Grade::FIRST, "First"},
            {Grade::SECOND, "SECOND"},
            {Grade::THIRD, "THIRD"},
            {Grade::FOURTH, "FOURTH"},
            {Grade::FIFTH, "FIFTH"},
            {Grade::SIXTH, "SIXTH"},
            {Grade::SEVENTH, "SEVENTH"},
            {Grade::EIGTH, "EIGTH"},
            {Grade::NINTH, "NINTH"},
            {Grade::TENTH, "TENTH"},
            {Grade::ELEVENTH, "ELEVENTH"},
            {Grade::TWELTH, "TWELTH"}
    }

};

#endif //MISIM_SCHOOL_H
