//
// Created by Vi1i on 11/28/16.
//

#include "../headers/DBConnetionManager.h"

DBConnectionManager::DBConnectionManager(std::string dbname, std::string server,
                                         std::string username,
                                         std::string password) {
    this->conn       = new mysqlpp::Connection(true);
    this->_dbname    = dbname;
    this->_server    = server;
    this->_username  = username;
    this->_password  = password;
}

DBConnectionManager::~DBConnectionManager() {
    delete this->conn;
    this->conn = NULL;
}

bool DBConnectionManager::ConnectionCheck() {
    this->conn->connect(this->_dbname.c_str(), this->_server.c_str(),
                        this->_username.c_str(), this->_password.c_str());
    return this->conn->connected();
}

bool DBConnectionManager::QueryCheck(std::string q_str) {
    this->_query_str = q_str;
}

bool DBConnectionManager::Query() {
    this->_query = new mysqlpp::Query(this->conn->query(this->_query_str));
    this->_store_query_result = this->_query->store();
    this->_query_iterator = this->_store_query_result.begin();
}

std::vector<std::string> DBConnectionManager::GetRow() {
    int counter = 0;
    std::vector<std::string> data;
    mysqlpp::Row row = *(this->_query_iterator);

    for(mysqlpp::Row::iterator it = row.begin(); it != row.end(); ++it) {
        std::string temp_s = (*it).c_str();
        data.push_back(temp_s);
    }

    return data;
}

bool DBConnectionManager::NextRow() {
    this->_query_iterator++;
}

bool DBConnectionManager::PrevRow() {
    this->_query_iterator--;
}