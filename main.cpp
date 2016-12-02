#include <iostream>
#include <vector>
#include "headers/DBConnetionManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string dbname   = "";
    std::string server   = "";
    std::string username = "";
    std::string password = "";

    DBConnectionManager connectionManager = DBConnectionManager(
            dbname, server, username, password);

    if(connectionManager.ConnectionCheck()) {
        connectionManager.QueryCheck("SELECT * FROM baseEntityMaster1314");
        connectionManager.Query();
        std::vector<std::string> row = connectionManager.GetRow();

        for(auto it = row.begin(); it != row.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }else{
        std::cerr << "Failed!" << std::endl;
    }
    return 0;
}
