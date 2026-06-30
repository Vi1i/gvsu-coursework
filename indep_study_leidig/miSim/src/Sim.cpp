//
// Created by Vi1i on 11/28/16.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "../headers/Sim.h"

Sim::Sim(int length_of_sim, int time_interval, unsigned int seed, std::string filename_in) {
    this->_length_of_sim = length_of_sim;
    this->_time_interval = time_interval;
    this->_seed = seed;
    this->_filename_in = filename_in;
    this->_filename_out = "out-" + this->_filename_in.substr(this->_filename_in.find_last_of("\\/") + 1);
    this->_cur_day = Sim::SUNDAY;
    this->_days = 0;
    this->_total_pop = 0;


    srand(this->_seed);
}

Sim::~Sim() {

}

void Sim::PrintProgress(double percentage) {

    const std::string PBSTR = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
    const int PBWIDTH = 80;

    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    fprintf(stdout, "\r%3d%% [%.*s%*s]", val, lpad, PBSTR.c_str(), rpad, "");
    fflush (stdout);
}

void Sim::Run() {
    int prev_day = 0;
    //std::cout << "Start of day: " << this->_days << std::endl;
    while(this->_days <= this->_length_of_sim){

        this->Step();
/*
        int dead = 0;
        int alive = 0;
        int infected = 0;
        std::cout << std::endl;
        for(auto school : this->_schools) {
            for (auto grade : school.second) {
                std::vector<Person>::iterator it;
                for (it = grade.second.begin(); it != grade.second.end(); ++it) {
                    if(it->Dead()) {
                        dead++;
                    }else{
                        if(it->Infected()) {
                            infected++;
                        }
                        alive++;
                    }
                }
            }
        }
        std::cout << this->_days << "," << alive << "," << infected << "," << dead << std::endl;
*/

        if(this->_days > prev_day) {
            this->PrintProgress(this->_days / (double)this->_length_of_sim);
            prev_day++;
        }
    }
    std::cout << std::endl;
}

bool Sim::Init() {
    int infected_count = 0;
    unsigned int cur_pop = 0;
    int prev_perc = 1;

    for(auto school : this->_bucket){ //Create per school
        for(auto grade : school.second) { //Create per grade
            std::vector<Person> persons;
            for(int z = 0; z < grade.second; z++) { //Create per individual
                double r = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
                double r2 = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
                bool infected = r > .99;
                bool vaccinated = r2 < .99;
                if(infected) {
                    infected_count++;
                }
                Person person = Person(cur_pop, new Tuberculosis(infected, vaccinated));

                persons.push_back(person);

                double percentage = (double) cur_pop / this->_total_pop;
                if ((percentage * 100) > prev_perc) {
                    this->PrintProgress(percentage);
                    prev_perc = (int) std::ceil(percentage * 100);
                }
                cur_pop++;
            }

            if(this->_schools.find(school.first) == this->_schools.end()) {
                // key doesn't exist
                this->_schools[school.first] = std::map<int, std::vector<Person>>();
            }
            this->_schools[school.first][grade.first] = persons;
        }
    }
    this->PrintProgress(1);
    std::cout << std::endl;
    //std::cout << infected_count << "/" << this->_total_pop << std::endl;
    return true;
}

bool Sim::Read() {
    int count = 0;
    int grade_count[12];
    std::string line;
    std::ifstream my_file(this->_filename_in);
    if(my_file.is_open()) {
        int pos = 0;
        while(std::getline(my_file, line)) {
            if(line[0] == '#') {
                count++;
                pos = 0;
            }else if(count == 1) {
            }else if(count == 2) { //Get grade data
                int counter = 0;
                std::istringstream iss(line);
                std::string token;

                while (std::getline(iss, token, ':')) {
                    if(counter != 0 && counter % 2 != 0) {
                        int val = (int) std::strtol(token.c_str(), nullptr, 10);
                        grade_count[pos] = val;
                        pos++;
                    }
                    counter++;
                }
            }else if(count == 3){ //Get
                //std::cout << line << std::endl;
                std::istringstream iss(line);
                std::string token;

                int counter = 0;

                std::string b_number;
                int grade = 0;
                int grade_val = 0;

                while (std::getline(iss, token, ':')) {
                    if(counter == 0) { //Building code
                        b_number = token;
                    }else if(counter == 2) { //Grade
                        grade = (int) std::strtol(token.c_str(), nullptr, 10);
                    }else if(counter == 4) { //Percentage of Pop
                        std::string::size_type sz;
                        int val = (int) std::ceil(grade_count[grade] * std::stof(token.c_str(), &sz));
                        grade_val = val;
                        this->_total_pop += val;
                    }
                    counter++;
                }
                //Add to the population data set
                if(this->_bucket.find(b_number) == this->_bucket.end()) {
                    this->_bucket[b_number] = std::map<int,int>();
                }
                this->_bucket[b_number][grade] = grade_val;
            }
        }
        my_file.close();
    }else{
        return false;
    }

    return true;
}

bool Sim::Write() {
    std::ofstream out;
    out.open(this->_filename_out);
    int dead = 0;
    int alive = 0;
    int infected = 0;

    for(auto school : this->_schools) {
        for (auto grade : school.second) {
            for (auto &person : grade.second) {
                if(person.Dead()) {
                    dead++;
                }else{
                    if(person.Infected()) {
                        infected++;
                    }
                    alive++;
                }

                out << school.first << "," << grade.first << "," << (person.Dead() ? "Dead" : "Alive") << std::endl;
            }
        }
    }
    out << "#Simulated sim data" << std::endl;
    out << "Days simulated," << this->_length_of_sim << std::endl;
    out << "Time interval(m)," <<  this->_time_interval << std::endl;
    out << "Seed for srand," << this->_seed << std::endl;
    out << "Total population," << this->_total_pop << std::endl;
    out << "Population(Alive)," << alive << std::endl;
    out << "Population(Dead)," << dead << std::endl;
    out << "Population(Infected)," << infected << std::endl;

    out.close();
    return true;
}

void Sim::Step() {
    //std::cout << "Current Time: " << this->_time << std::endl;

    this->_time += this->_time_interval;

    if (this->_time > Sim::END_OF_DAY) {
        //std::cout << "End of day: " << this->_days << std::endl;
        this->_days++;
        this->NextDay();
        this->_time -= Sim::END_OF_DAY;
        //std::cout << "Start of day: " << this->_days << std::endl;
    }

    if(this->_cur_day != Sim::SATURDAY && this->_cur_day != Sim::SUNDAY) {
        if (this->_time >= Sim::SCHOOL_START && this->_time < Sim::SCHOOL_END) {
            if (this->_time - Sim::SCHOOL_START <= 0) {
                //std::cout << "School started" << std::endl;
            }
            if (this->_time >= Sim::LUNCH_END && this->_time - Sim::LUNCH_END <= 0) {
                //std::cout << "Lunch ended" << std::endl;
            }

            if (this->_time >= Sim::LUNCH_START && this->_time < Sim::LUNCH_END) {
                if (this->_time - Sim::LUNCH_START <= 0) {
                    //std::cout << "Lunch started" << std::endl;
                }
                // Per school step
                for(auto school : this->_schools) {
                    for(auto grade : school.second) {
                        for(auto &person : grade.second) {
                            if(!person.Dead()) {
                                if (person.Infected()) {
                                    double chance = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
                                    person.Step(this->_days, chance);
                                }
                                if (person.CanInfect()) {
                                    for (auto grade2 : school.second) {
                                        for (auto &per : grade2.second) {
                                            if(!per.Dead()) {
                                                if (person != per) {
                                                    double chance =
                                                            static_cast <float> (std::rand()) /
                                                            static_cast <float> (RAND_MAX);
                                                    per.Step(this->_days, chance);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                return;
            }
            // Per class step
            for(auto school : this->_schools) {
                for(auto grade : school.second) {
                    for(auto &person : grade.second) {
                        if(!person.Dead()) {
                            if (person.Infected()) {
                                double chance = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
                                person.Step(this->_days, chance);
                            }
                            if (person.CanInfect()) {
                                for (auto &per : grade.second) {
                                    if(!per.Dead()) {
                                        if (person != per) {
                                            double chance =
                                                    static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
                                            per.Step(this->_days, chance);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return;
        }
        if (this->_time >= Sim::SCHOOL_END && this->_time - Sim::SCHOOL_END <= 0) {
            //std::cout << "School ended" << std::endl;
        }
    }
    return;

}

void Sim::NextDay() {
    if(this->_cur_day == Sim::SATURDAY) {
        this->_cur_day = Sim::SUNDAY;
    }else{
        this->_cur_day++;
    }
}