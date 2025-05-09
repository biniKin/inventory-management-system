#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

// Initialize database and create tables if needed
bool initializeDatabase(sqlite3*& db);

// Product database operations
bool addProductDB(sqlite3* db, int id, const std::string& name, const std::string& category, double price, int quantity);
bool removeProductDB(sqlite3* db, int id);
bool findProductDB(sqlite3* db, int id);
bool updateProductDB(sqlite3* db, int id, const std::string& name, const std::string& category, double price, int quantity);
bool printProductsDB(sqlite3* db);

// Close database connection
void closeDatabase(sqlite3* db);

#endif // DATABASE_H

