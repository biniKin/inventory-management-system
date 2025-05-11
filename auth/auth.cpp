#include <iostream>
#include <cstdlib>
#include <string>
#include <limits>
#include "auth.h"

using namespace std;

UserRole authUser(sqlite3* db) {
    string username;
    string password;
    int trial = 3;
    char choice;

    while (true) {
        cout << "\nLOGIN MENU" << endl;
        cout << "---------------------" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register new account" << endl;
        cout << "Q. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == '1') {
            // Login
            while (trial > 0) {
                cout << "\nLOGIN" << endl;
                cout << "---------------------" << endl;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);

                UserRole role = authenticateUser(db, username, password);
                
                if (role == ROLE_ADMIN) {
                    cout << "Login successful as Administrator!" << endl;
                    return ROLE_ADMIN;
                } 
                else if (role == ROLE_USER) {
                    cout << "Login successful as User!" << endl;
                    return ROLE_USER;
                }
                else {
                    trial--;
                    if (trial == 0) {
                        cout << "Access denied. Too many failed attempts." << endl;
                        exit(0);
                    } else {
                        cout << "Wrong credentials. You have " << trial << " trial(s) left." << endl;
                    }
                }
            }
        } else if (choice == '2') {
            // Register
            registerNewUser(db);
        } else if (choice == 'Q' || choice == 'q') {
            cout << "Goodbye!" << endl;
            exit(0);
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return ROLE_NONE; // This should never be reached
}

void registerNewUser(sqlite3* db) {
    string username, password, confirmPassword;
    
    cout << "\nREGISTER NEW ACCOUNT" << endl;
    cout << "---------------------" << endl;
    
    while (true) {
        cout << "Enter desired username: ";
        getline(cin, username);
        
        if (username.empty()) {
            cout << "Username cannot be empty. Please try again." << endl;
            continue;
        }
        
        if (checkUsernameExists(db, username)) {
            cout << "Username already exists. Please choose a different username." << endl;
            continue;
        }
        
        break;
    }
    
    while (true) {
        cout << "Enter password: ";
        getline(cin, password);
        
        if (password.empty()) {
            cout << "Password cannot be empty. Please try again." << endl;
            continue;
        }
        
        cout << "Confirm password: ";
        getline(cin, confirmPassword);
        
        if (password != confirmPassword) {
            cout << "Passwords do not match. Please try again." << endl;
            continue;
        }
        
        break;
    }
    
    // New users are always registered as regular users
    if (registerUser(db, username, password, "user")) {
        cout << "Registration successful! You can now login with your new account." << endl;
    } else {
        cout << "Registration failed. Please try again later." << endl;
    }
}

void manageUsers(sqlite3* db) {
    char choice;
    
    while (true) {
        cout << "\nUSER MANAGEMENT" << endl;
        cout << "---------------------" << endl;
        cout << "1. List all users" << endl;
        cout << "2. Add new user" << endl;
        cout << "3. Delete user" << endl;
        cout << "4. Return to main menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case '1': {
                listAllUsers(db);
                break;
            }
            case '2': {
                string username, password, role;
                
                cout << "\nADD NEW USER" << endl;
                cout << "---------------------" << endl;
                
                while (true) {
                    cout << "Enter username: ";
                    getline(cin, username);
                    
                    if (username.empty()) {
                        cout << "Username cannot be empty. Please try again." << endl;
                        continue;
                    }
                    
                    if (checkUsernameExists(db, username)) {
                        cout << "Username already exists. Please choose a different username." << endl;
                        continue;
                    }
                    
                    break;
                }
                
                cout << "Enter password: ";
                getline(cin, password);
                
                while (true) {
                    cout << "Enter role (admin/user): ";
                    getline(cin, role);
                    
                    if (role != "admin" && role != "user") {
                        cout << "Invalid role. Please enter 'admin' or 'user'." << endl;
                        continue;
                    }
                    
                    break;
                }
                
                registerUser(db, username, password, role);
                break;
            }
            case '3': {
                string username;
                
                cout << "\nDELETE USER" << endl;
                cout << "---------------------" << endl;
                listAllUsers(db);
                
                cout << "Enter username to delete: ";
                getline(cin, username);
                
                deleteUser(db, username);
                break;
            }
            case '4':
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}



