#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>


enum UserRole {
    ROLE_ADMIN,
    ROLE_USER,
    ROLE_NONE  
};


bool initializeDatabase(sqlite3*& db);

UserRole authenticateUser(sqlite3* db, const std::string& username, const std::string& password);
bool registerUser(sqlite3* db, const std::string& username, const std::string& password, const std::string& role);
bool checkUsernameExists(sqlite3* db, const std::string& username);
bool listAllUsers(sqlite3* db);
bool deleteUser(sqlite3* db, const std::string& username);


bool addProductDB(sqlite3* db, int id, const std::string& name, const std::string& category, double price, int quantity);
bool removeProductDB(sqlite3* db, int id);
bool findProductDB(sqlite3* db, int id);
bool updateProductDB(sqlite3* db, int id, const std::string& name, const std::string& category, double price, int quantity);
bool printProductsDB(sqlite3* db);
void showProductCount(sqlite3* db);  


void closeDatabase(sqlite3* db);

#endif 


