#include "product.h"
#include <iostream>
#include <sqlite3.h>
#include "../database/database.h"  // To access database functions

using namespace std;

const int MAX_PRODUCTS = 100;
Product products[MAX_PRODUCTS];
int productCount = 0;

void addProduct(int id, string name, string category, double price, int quantity) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            cout << "ID already exists." << endl;
            return;
        }
    }

    if (productCount < MAX_PRODUCTS) {
        products[productCount] = {id, name, category, price, quantity};
        productCount++;
        cout << "Product added successfully." << endl;
    } else {
        cout << "Inventory is full!" << endl;
    }
}

void removeProduct(int id) {
    bool found = false;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            for (int j = i; j < productCount - 1; j++) {
                products[j] = products[j + 1];
            }
            productCount--;
            found = true;
            cout << "Product removed successfully." << endl;
            break;
        }
    }

    if (!found) {
        cout << "ID does not exist." << endl;
    }
}

void findProduct(int id) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            cout << "ID: " << products[i].id << endl;
            cout << "Name: " << products[i].name << endl;
            cout << "Category: " << products[i].category << endl;
            cout << "Price: $" << products[i].price << endl;
            cout << "Quantity: " << products[i].quantity << endl;
            return;
        }
    }
    cout << "Product not found." << endl;
}

void updateProduct(int id, string name, string category, double price, int quantity) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            products[i].name = name;
            products[i].category = category;
            products[i].price = price;
            products[i].quantity = quantity;
            cout << "Product updated successfully." << endl;
            return;
        }
    }
    cout << "ID does not exist." << endl;
}

void showAllProducts() {
    sqlite3* db = nullptr;
    if (!initializeDatabase(db)) {
        std::cout << "Failed to connect to database." << std::endl;
        return;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT COUNT(*) FROM products;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        closeDatabase(db);
        return;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        std::cout << "\nTotal Products: " << count << std::endl;
    } else {
        std::cout << "Error retrieving product count." << std::endl;
    }
    
    sqlite3_finalize(stmt);
    closeDatabase(db);
}

void printProducts() {
    for (int i = 0; i < productCount; i++) {
        cout << "ID: " << products[i].id << endl;
        cout << "Name: " << products[i].name << endl;
        cout << "Category: " << products[i].category << endl;
        cout << "Price: $" << products[i].price << endl;
        cout << "Quantity: " << products[i].quantity << endl;
        cout << "------------------------" << endl;
    }
}
