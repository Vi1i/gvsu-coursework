//
// Created by Vi1i on 11/28/16.
//

#ifndef MISIM_SIM_H
#define MISIM_SIM_H

#include <string>
#include <ctime>
#include <vector>
#include <ctime>

#include "../headers/Person.h"


class Sim {
public:
    enum SCHOOL {
        START_OF_DAY = 0,
        SCHOOL_START = 480,
        LUNCH_START  = 720,
        LUNCH_END    = 765,
        SCHOOL_END   = 900,
        END_OF_DAY   = 1440
    };

    enum DAYS {
        SUNDAY    = 0,
        MONDAY    = 1,
        TUESDAY   = 2,
        WEDNESDAY = 3,
        THURSDAY  = 4,
        FRIDAY    = 5,
        SATURDAY  = 6
    };

    Sim(int length_of_sim, int time_interval, unsigned int seed, std::string filename_in);
    ~Sim();

    bool Init();
    void Run();
    bool Read();
    bool Write();
    void NextDay();
private:
    void Step();
    void PrintProgress (double percentage);

    std::string _filename_in;
    std::string _filename_out;
    unsigned int _seed;
    int _length_of_sim;  // Amount of days for sime to run;
    int _cur_day;
    int _days;           // Since Sim started;
    int _time;           // In minutes
    int _time_interval;  // In minutes
    int _total_pop;
    std::map<std::string, std::map<int, int>> _bucket;
    std::map<std::string, std::map<int, std::vector<Person>>> _schools;
};

#endif //MISIM_SIM_H