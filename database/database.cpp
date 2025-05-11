#include "database.h"
#include <iostream>
#include <string>
#include <iomanip> 

bool initializeDatabase(sqlite3*& db) {
    char* errMsg = nullptr;
    int rc = sqlite3_open("inventory.db", &db);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // Create products table if it doesn't exist
    const char* createProductsTableSQL = 
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "category TEXT,"
        "price REAL,"
        "quantity INTEGER"
        ");";
        
    rc = sqlite3_exec(db, createProductsTableSQL, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    // Create users table if it doesn't exist
    const char* createUsersTableSQL = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "role TEXT NOT NULL"
        ");";
        
    rc = sqlite3_exec(db, createUsersTableSQL, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    // Check if default users exist, if not add them
    const char* checkUsersSQL = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, checkUsersSQL, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    rc = sqlite3_step(stmt);
    int userCount = 0;
    
    if (rc == SQLITE_ROW) {
        userCount = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    
    // Add default users if none exist
    if (userCount == 0) {
        const char* addDefaultUsersSQL = 
            "INSERT INTO users (username, password, role) VALUES "
            "('admin', '123', 'admin'),"
            "('user', '456', 'user');";
            
        rc = sqlite3_exec(db, addDefaultUsersSQL, nullptr, nullptr, &errMsg);
        
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
        
        std::cout << "Default users created." << std::endl;
    }
    
    std::cout << "Database initialized successfully" << std::endl;
    return true;
}

UserRole authenticateUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT role FROM users WHERE username = ? AND password = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return ROLE_NONE;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    
    UserRole role = ROLE_NONE;
    
    if (rc == SQLITE_ROW) {
        std::string roleStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (roleStr == "admin") {
            role = ROLE_ADMIN;
        } else if (roleStr == "user") {
            role = ROLE_USER;
        }
    }
    
    sqlite3_finalize(stmt);
    return role;
}




