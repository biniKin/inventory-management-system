#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <sqlite3.h>

struct Product {
    int id;
    std::string name;
    std::string category;
    double price;
    int quantity;
};

// Function declarations
void addProduct(int id, std::string name, std::string category, double price, int quantity);
void removeProduct(int id);
void findProduct(int id);
void updateProduct(int id, std::string name, std::string category, double price, int quantity);
void showAllProducts();  // Added this declaration
void printProducts();

#endif // PRODUCT_H
