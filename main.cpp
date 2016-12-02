#include <iostream>
#include "headers/DBConnetionManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    DBConnectionManager connectionManager = DBConnectionManager(
            "EntityMaster1314", "localhost", "root", "Voidtak3?");

    if(connectionManager.ConnectionCheck()) {
        connectionManager.QueryCheck("SELECT * FROM baseEntityMaster1314");
        connectionManager.Query();
        std::cerr << "Passed!" << std::endl;
        std::string * row = connectionManager.GetRow();

        for(auto it = row->begin(); it != row->end(); ++it) {
            std::cout << *it << std::endl;
        }
    }else{
        std::cerr << "Failed!" << std::endl;
    }
    return 0;
}