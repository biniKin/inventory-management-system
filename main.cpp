#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "headerText/headerText.h"
#include "auth/auth.h"
#include "database/database.h"
#include "product/product.h"
#include <sqlite3.h>

using namespace std;

int main(){
    header(); 
    authUser();

    sqlite3* db = nullptr;
    if (!initializeDatabase(db)) {
        cout << "Failed to initialize database. Exiting..." << endl;
        return 1;
    }

    // main menu
    char choice;
    do {
        cout << "\nINVENTORY MANAGEMENT SYSTEM" << endl;
        cout << "----------------------------" << endl;
        cout << "1. Add a product" << endl;
        cout << "2. Remove a product" << endl;
        cout << "3. Find a product" << endl;
        cout << "4. Update a product" << endl;
        cout << "5. View all products" << endl;
        cout << "6. Show total product count" << endl;
        cout << "Q. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the newline character
        
        switch (choice) {
        case '1': {
            int id, quantity;
            string name, category;
            double price;
            cout << "Enter ID: ";
            cin >> id;
            cin.ignore(); // Clear the newline character
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Category: ";
            getline(cin, category);
            cout << "Enter Price: ";
            cin >> price;
            cout << "Enter Quantity: ";
            cin >> quantity;
            addProductDB(db, id, name, category, price, quantity);
            break;
        }
        case '2': {
            int id;
            cout << "Enter ID to remove: ";
            cin >> id;
            removeProductDB(db, id);
            break;
        }
        case '3': {
            int id;
            cout << "Enter ID to find: ";
            cin >> id;
            findProductDB(db, id);
            break;
        }
        case '4': {
            int id, quantity;
            string name, category;
            double price;
            cout << "Enter ID to update: ";
            cin >> id;
            cin.ignore(); // Clear the newline character
            cout << "Enter new Name: ";
            getline(cin, name);
            cout << "Enter new Category: ";
            getline(cin, category);
            cout << "Enter new Price: ";
            cin >> price;
            cout << "Enter new Quantity: ";
            cin >> quantity;
            updateProductDB(db, id, name, category, price, quantity);
            break;
        }
        case '5':
            printProductsDB(db);
            break;
        case '6':
            showAllProducts();
            break;
        case 'Q':
        case 'q':
            closeDatabase(db);
            // Close the database connection before exiting
            cout << "Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice." << endl;
        }
    } while (true);

    // This should never be reached, but just in case
    closeDatabase(db);
    return 0;
}
