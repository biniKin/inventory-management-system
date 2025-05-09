#include "database.h"
#include <iostream>

bool initializeDatabase(sqlite3*& db) {
    char* errMsg = nullptr;
    int rc = sqlite3_open("inventory.db", &db);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // Create products table if it doesn't exist
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "category TEXT,"
        "price REAL,"
        "quantity INTEGER"
        ");";
        
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    std::cout << "Database initialized successfully" << std::endl;
    return true;
}

// Callback function for displaying query results
static int displayCallback(void* data, int argc, char** argv, char** colNames) {
    for (int i = 0; i < argc; i++) {
        std::cout << colNames[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << "------------------------" << std::endl;
    return 0;
}

bool addProductDB(sqlite3* db, int id, const std::string& name, const std::string& category, double price, int quantity) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO products (id, name, category, price, quantity) VALUES (?, ?, ?, ?, ?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, price);
    sqlite3_bind_int(stmt, 5, quantity);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    std::cout << "Product added successfully." << std::endl;
    return true;
}

bool findProductDB(sqlite3* db, int id) {
    char* errMsg = nullptr;
    std::string sql = "SELECT * FROM products WHERE id = " + std::to_string(id) + ";";
    
    int rc = sqlite3_exec(db, sql.c_str(), displayCallback, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool removeProductDB(sqlite3* db, int id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM products WHERE id = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    if (sqlite3_changes(db) > 0) {
        std::cout << "Product removed successfully." << std::endl;
        return true;
    } else {
        std::cout << "Product not found." << std::endl;
        return false;
    }
}

bool updateProductDB(sqlite3* db, int id, const std::string& name, const std::string& category, double price, int quantity) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE products SET name = ?, category = ?, price = ?, quantity = ? WHERE id = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, price);
    sqlite3_bind_int(stmt, 4, quantity);
    sqlite3_bind_int(stmt, 5, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    if (sqlite3_changes(db) > 0) {
        std::cout << "Product updated successfully." << std::endl;
        return true;
    } else {
        std::cout << "Product not found." << std::endl;
        return false;
    }
}

bool printProductsDB(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM products;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    bool found = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;
        std::cout << "ID: " << sqlite3_column_int(stmt, 0) << std::endl;
        std::cout << "Name: " << (const char*)sqlite3_column_text(stmt, 1) << std::endl;
        std::cout << "Category: " << (const char*)sqlite3_column_text(stmt, 2) << std::endl;
        std::cout << "Price: $" << sqlite3_column_double(stmt, 3) << std::endl;
        std::cout << "Quantity: " << sqlite3_column_int(stmt, 4) << std::endl;
        std::cout << "------------------------" << std::endl;
    }
    
    sqlite3_finalize(stmt);
    
    if (!found) {
        std::cout << "No products found in the database." << std::endl;
    }
    
    return true;
}

void closeDatabase(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database connection closed." << std::endl;
    }
}






