#include <iostream>
#include <cstdlib> // C++ standard library for using "exit()"
#include "auth.h" // include the header file for authUser function

using namespace std;

// define function authuser
void authUser() {
    const string correctUserName = "admin";
    const int correctPassword = 123;
    int trial = 3;
    string username;
    int password;

    cout << "Login" << endl;

    while (trial > 0) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (username == correctUserName && password == correctPassword) {
            cout << "Login successful!" << endl;
            return;
        } else {
            trial--;
            if (trial == 0) {
                cout << "Access denied." << endl;
                exit(0);
            } else {
                cout << "Wrong credentials. You have " << trial << " trial(s) left." << endl;
            }
        }
    }
}
