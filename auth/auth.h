#ifndef AUTHUSER_H
#define AUTHUSER_H

#include <sqlite3.h>
#include "../database/database.h"

// Returns the role of the authenticated user
UserRole authUser(sqlite3* db);

// User registration
void registerNewUser(sqlite3* db);

// User management (admin only)
void manageUsers(sqlite3* db);

#endif
