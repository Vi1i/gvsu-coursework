//
// Created by Vi1i on 11/28/16.
//

#ifndef MISIM_DBCONNETIONMANAGER_H
#define MISIM_DBCONNETIONMANAGER_H

#include <mysql++.h>
#include <mysql.h>

class DBConnectionManager {
public:
    DBConnectionManager(std::string dbname, std::string server,
                        std::string username, std::string password);
    ~DBConnectionManager();

    bool ConnectionCheck();
    bool QueryCheck(std::string q_str);
    bool  Query();
    std::vector<std::string> GetRow();
    bool PrevRow();
    bool NextRow();

private:
    mysqlpp::Connection * conn;
    mysqlpp::Query * _query;
    mysqlpp::StoreQueryResult _store_query_result;
    mysqlpp::StoreQueryResult::iterator _query_iterator;
    std::string _dbname;
    std::string _server;
    std::string _username;
    std::string _password;
    std::string _query_str;
};

#endif //MISIM_DBCONNETIONMANAGER_H
