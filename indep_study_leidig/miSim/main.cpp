#include <iostream>
#include <vector>
#include "headers/DBConnetionManager.h"
#include "headers/Sim.h"

namespace Test {
    inline bool DBConnectionManagerTest() {
        std::string dbname   = "";
        std::string server   = "";
        std::string username = "";
        std::string password = "";

    try {
        DBConnectionManager connectionManager = DBConnectionManager(
                dbname, server, username, password);

        if (connectionManager.ConnectionCheck()) {
            connectionManager.QueryCheck("SELECT * FROM baseEntityMaster1314");
            connectionManager.Query();
            std::vector<std::string> row = connectionManager.GetRow();

            for (auto it = row.begin(); it != row.end(); ++it) {
                std::cout << *it << std::endl;
            }
        } else {
            return false;
        }
    }catch(std::exception e) {
        return false;
    }

        return true;
    }

    inline bool SimTest() {
        int length_of_sim = 365;
        int time_interval = 15;
        unsigned int seed = 2;
        std::string filename_in = "../school-population-breakdown";
        Sim sim = Sim(length_of_sim, time_interval, seed,filename_in);

        std::cout << "Reading data" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        sim.Read();
        auto end = std::chrono::high_resolution_clock::now();
        auto read_time = end - start;
        std::cout << "Read Time(s): " << (std::chrono::duration_cast<std::chrono::nanoseconds>(read_time).count()) /1000000000.0
                  << "s" << std::endl;

        std::cout << "Initializing data" << std::endl;
        start = std::chrono::high_resolution_clock::now();
        sim.Init();
        end = std::chrono::high_resolution_clock::now();
        auto init_time = end - start;
        std::cout << "Init Time(s): " << (std::chrono::duration_cast<std::chrono::nanoseconds>(init_time).count()) /1000000000.0
                  << "s" << std::endl;

        std::cout << "Running simulation" << std::endl;
        start = std::chrono::high_resolution_clock::now();
        sim.Run();
        end = std::chrono::high_resolution_clock::now();
        auto run_time = end - start;
        std::cout << "Running Time(s): " << (std::chrono::duration_cast<std::chrono::nanoseconds>(run_time).count()) /1000000000.0
                  << "s" << std::endl;

        std::cout << "Writing data" << std::endl;
        start = std::chrono::high_resolution_clock::now();
        sim.Write();end = std::chrono::high_resolution_clock::now();
        auto write_time = end - start;
        std::cout << "WriteTime(s): " << (std::chrono::duration_cast<std::chrono::nanoseconds>(write_time).count()) /1000000000.0
                  << "s" << std::endl;

        return true;
    }
}

int main() {

    //if(!Test::DBConnectionManagerTest()) {
    //    std::cerr << "F";
    //}else{
    //    std::cout << ".";
    //}

    Test::SimTest();

    std::cout << std::endl;
    return 0;
}
