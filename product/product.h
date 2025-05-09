#ifndef PRODUCT_H // if product not defined it will move to the next
#define PRODUCT_H // define product

#include <string>
#include <sqlite3.h> // used for sqlite3 database

using namespace std;

// Product structure
struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

// Function declarations
void addProduct(int id, string name, string category, double price, int quantity);
void removeProduct(int id);
void findProduct(int id);
void updateProduct(int id, string name, string category, double price, int quantity);
void showAllProducts();  
void printProducts();

#endif // PRODUCT_H
