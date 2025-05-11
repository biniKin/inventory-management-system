#include "database.h"
#include <iostream>
#include <string>
#include <iomanip> 

bool initializeDatabase(sqlite3 *&db)
{
    char *errMsg = nullptr;
    int rc = sqlite3_open("inventory.db", &db);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl; 
        return false;
    }

    
    const char *createProductsTableSQL =
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "category TEXT,"
        "price REAL,"
        "quantity INTEGER"
        ");";

    rc = sqlite3_exec(db, createProductsTableSQL, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    
    const char *createUsersTableSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "role TEXT NOT NULL"
        ");";

    rc = sqlite3_exec(db, createUsersTableSQL, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

   
    const char *checkUsersSQL = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, checkUsersSQL, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    rc = sqlite3_step(stmt);
    int userCount = 0;

    if (rc == SQLITE_ROW)
    {
        userCount = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

   
    if (userCount == 0)
    {
        const char *addDefaultUsersSQL =
            "INSERT INTO users (username, password, role) VALUES "
            "('admin', '123', 'admin'),"
            "('user', '456', 'user');";

        rc = sqlite3_exec(db, addDefaultUsersSQL, nullptr, nullptr, &errMsg);

        if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }

        std::cout << "Default users created." << std::endl;
    }

    std::cout << "Database initialized successfully" << std::endl;
    return true;
}

UserRole authenticateUser(sqlite3 *db, const std::string &username, const std::string &password)
{
    sqlite3_stmt *stmt; 
    const char *sql = "SELECT role FROM users WHERE username = ? AND password = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return ROLE_NONE;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC); 
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt); 

    UserRole role = ROLE_NONE;

    if (rc == SQLITE_ROW)
    {
        std::string roleStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        if (roleStr == "admin")
        {
            role = ROLE_ADMIN;
        }
        else if (roleStr == "user")
        {
            role = ROLE_USER;
        }
    }

    sqlite3_finalize(stmt);
    return role;
}


bool addProductDB(sqlite3 *db, int id, const std::string &name, const std::string &category, double price, int quantity)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO products (id, name, category, price, quantity) VALUES (?, ?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
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

    if (rc != SQLITE_DONE) 
    {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::cout << "Product added successfully. called on db" << std::endl;
    return true;
}

bool findProductDB(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM products WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    bool found = false;

    
    std::cout << "\n+-------+----------------------+----------------------+------------+------------+" << std::endl;
    std::cout << "| ID    | Name                 | Category             | Price      | Quantity   |" << std::endl;
    std::cout << "+-------+----------------------+----------------------+------------+------------+" << std::endl;

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = true;

        int productId = sqlite3_column_int(stmt, 0);
        const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *category = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        double price = sqlite3_column_double(stmt, 3);
        int quantity = sqlite3_column_int(stmt, 4);

       
        std::cout << "| " << std::left << std::setw(5) << productId << " | "
                  << std::setw(20) << (name ? std::string(name).substr(0, 20) : "NULL") << " | "
                  << std::setw(20) << (category ? std::string(category).substr(0, 20) : "NULL") << " | $"
                  << std::right << std::setw(9) << std::fixed << std::setprecision(2) << price << " | "
                  << std::setw(10) << quantity << " |" << std::endl;
    }

   
    std::cout << "+-------+----------------------+----------------------+------------+------------+" << std::endl;

    sqlite3_finalize(stmt);

    if (!found)
    {
        std::cout << "Product with ID " << id << " not found." << std::endl;
    }

    return found;
}

bool removeProductDB(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM products WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_changes(db) > 0)
    {
        std::cout << "Product removed successfully." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Product not found." << std::endl;
        return false;
    }
}

bool updateProductDB(sqlite3 *db, int id, const std::string &name, const std::string &category, double price, int quantity)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE products SET name = ?, category = ?, price = ?, quantity = ? WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
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

    if (rc != SQLITE_DONE)
    {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_changes(db) > 0)
    {
        std::cout << "Product updated successfully." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Product not found." << std::endl;
        return false;
    }
}

bool printProductsDB(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM products;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    bool found = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = true;
        std::cout << "ID: " << sqlite3_column_int(stmt, 0) << std::endl;
        std::cout << "Name: " << (const char *)sqlite3_column_text(stmt, 1) << std::endl;
        std::cout << "Category: " << (const char *)sqlite3_column_text(stmt, 2) << std::endl;
        std::cout << "Price: $" << sqlite3_column_double(stmt, 3) << std::endl;
        std::cout << "Quantity: " << sqlite3_column_int(stmt, 4) << std::endl;
        std::cout << "------------------------" << std::endl;
    }

    sqlite3_finalize(stmt);

    if (!found)
    {
        std::cout << "No products found in the database." << std::endl;
    }

    return true;
}

void closeDatabase(sqlite3 *db)
{
    if (db)
    {
        sqlite3_close(db);
        std::cout << "Database connection closed." << std::endl;
    }
}

bool checkUsernameExists(sqlite3 *db, const std::string &username)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM users WHERE username = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int count = 0;

    if (rc == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

bool registerUser(sqlite3 *db, const std::string &username, const std::string &password, const std::string &role)
{
    
    if (checkUsernameExists(db, username))
    {
        std::cerr << "Username already exists. Please choose a different username." << std::endl;
        return false;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (username, password, role) VALUES (?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "User registered successfully!" << std::endl;
    return true;
}

bool listAllUsers(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT username, role FROM users ORDER BY role, username;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::cout << "\nRegistered Users:" << std::endl;
    std::cout << "+----------------------+----------+" << std::endl;
    std::cout << "| Username             | Role     |" << std::endl;
    std::cout << "+----------------------+----------+" << std::endl;

    bool found = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = true;
        std::string username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        std::string role = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        std::cout << "| " << std::left << std::setw(20) << username << " | "
                  << std::setw(8) << role << " |" << std::endl;
    }

    std::cout << "+----------------------+----------+" << std::endl;

    sqlite3_finalize(stmt);

    if (!found)
    {
        std::cout << "No users found in the database." << std::endl;
    }

    return true;
}

bool deleteUser(sqlite3 *db, const std::string &username)
{
    
    if (username == "admin")
    {
        std::cerr << "Cannot delete the default admin account." << std::endl;
        return false;
    }

    
    if (!checkUsernameExists(db, username))
    {
        std::cerr << "User does not exist." << std::endl;
        return false;
    }

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM users WHERE username = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "User deleted successfully!" << std::endl;
    return true;
}

void showProductCount(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM products;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(stmt, 0);
        std::cout << "\n+------------------------+" << std::endl;
        std::cout << "| Total Products: " << std::left << std::setw(7) << count << " |" << std::endl;
        std::cout << "+------------------------+" << std::endl;
    }
    else
    {
        std::cout << "Error retrieving product count." << std::endl;
    }

    sqlite3_finalize(stmt);
}
