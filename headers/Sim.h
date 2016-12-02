//
// Created by Vi1i on 11/28/16.
//

#ifndef MISIM_SIM_H
#define MISIM_SIM_H

#include <string>

class Sim {
public:
    Sim(std::string filename_in);
    Sim(std::string filename_in, std::string filename_out);
    ~Sim();

    bool Init();
    void Run();
    bool Read();
    bool Write();
private:
    void Step();

    std::string filename_in;
    std::string filename_out;
};

#endif //MISIM_SIM_H
