#include <iostream>
#include <string>
#include <limits>
#include "headerText/headerText.h"
#include "auth/auth.h"
#include "database/database.h"
#include <sqlite3.h>

using namespace std;

int main() {
    header();
    
    sqlite3* db = nullptr;
    if (!initializeDatabase(db)) {
        cout << "Failed to initialize database. Exiting..." << endl;
        return 1;
    }
    
    UserRole role = authUser(db);

   
    char choice;
    do {
        cout << "\nINVENTORY MANAGEMENT SYSTEM" << endl;
        cout << "----------------------------" << endl;
        
        if (role == ROLE_ADMIN) {
           
            cout << "1. Add a product" << endl;
            cout << "2. Remove a product" << endl;
            cout << "3. Find a product" << endl;
            cout << "4. Update a product" << endl;
            cout << "5. View all products" << endl;
            cout << "6. Show total product count" << endl;
            cout << "7. Manage users" << endl;
            cout << "Q. Quit" << endl;
        } else {
            
            cout << "1. Find a product" << endl;
            cout << "2. View all products" << endl;
            cout << "3. Show total product count" << endl;
            cout << "Q. Quit" << endl;
        }
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        
        if (role == ROLE_ADMIN) {
           
            switch (choice) {
            case '1': {
                int id, quantity;
                string name, category;
                double price;
                cout << "Enter ID: ";
                cin >> id;
                cin.ignore(); 
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Category: ";
                getline(cin, category);
                cout << "Enter Price: ";
                cin >> price;
                cout << "Enter Quantity: ";
                cin >> quantity;
                if(price > 0 && quantity > 0){
                    addProductDB(db, id, name, category, price, quantity);
                } else{
                    cout << "Price and quantity must be positive values." << endl;
                }
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
                cin.ignore(); 
                cout << "Enter new Name: ";
                getline(cin, name);
                cout << "Enter new Category: ";
                getline(cin, category);
                cout << "Enter new Price: ";
                cin >> price;
                cout << "Enter new Quantity: ";
                cin >> quantity;
                if(price > 0 && quantity > 0){
                    updateProductDB(db, id, name, category, price, quantity);
                } else {
                    cout << "Price and quantity must be positive values." << endl;
                }
                break;
            }
            case '5':
                printProductsDB(db);
                break;
            case '6':
                showProductCount(db);  
                break;
            case '7':
                manageUsers(db);
                break;
            case 'Q':
            case 'q':
                closeDatabase(db);
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice." << endl;
            }
        } else {
            
            switch (choice) {
            case '1': {
                int id;
                cout << "Enter ID to find: ";
                cin >> id;
                findProductDB(db, id);
                break;
            }
            case '2':
                printProductsDB(db);
                break;
            case '3':
                showProductCount(db);  
                break;
            case 'Q':
            case 'q':
                closeDatabase(db);
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice." << endl;
            }
        }
    } while (true);

    
    closeDatabase(db);
    return 0;
}
