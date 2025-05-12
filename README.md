# Inventory Management System

A C++ console application for managing inventory with user authentication and SQLite database storage.

## Features

- **User Authentication**
  - Login system with admin and regular user roles
  - User registration
  - Admin user management (add/delete users)

- **Inventory Management**
  - Add, remove, update, and find products
  - View all products in a formatted table
  - Show total product count

- **Database Storage**
  - SQLite database for persistent data storage
  - Automatic database initialization
  - Default admin and user accounts

## Getting Started

### Prerequisites

- C++ compiler with C++17 support
- SQLite3 library
- MinGW (for Windows users)

### Installation

1. Clone the repository:
  https://github.com/biniKin/inventory-management-system.git

2. Navigate to the project directory:
   ```
   cd inventory-management-system
   ```

3. Compile the project:
   ```
   g++ -o app main.cpp auth/auth.cpp headerText/headerText.cpp database/database.cpp -lsqlite3
   ```

4. Run the application:
   ```
   ./app
   ```

### Default Credentials

- **Admin Account**
  - Username: admin
  - Password: 123

- **User Account**
  - Username: user
  - Password: 456

## Project Structure

- **main.cpp**: Main application entry point and menu system
- **auth/**: User authentication and management
  - **auth.h**: Authentication function declarations
  - **auth.cpp**: Authentication function implementations
- **database/**: Database operations
  - **database.h**: Database function declarations
  - **database.cpp**: Database function implementations
- **headerText/**: Application header display
  - **headerText.h**: Header function declaration
  - **headerText.cpp**: Header function implementation

## Usage

### Admin Functions

1. Add a product
2. Remove a product
3. Find a product
4. Update a product
5. View all products
6. Show total product count
7. Manage users

### User Functions

1. Find a product
2. View all products
3. Show total product count

## Database Schema

### Products Table

| Column   | Type    | Description           |
|----------|---------|-----------------------|
| id       | INTEGER | Primary key           |
| name     | TEXT    | Product name          |
| category | TEXT    | Product category      |
| price    | REAL    | Product price         |
| quantity | INTEGER | Available quantity    |

### Users Table

| Column   | Type    | Description           |
|----------|---------|-----------------------|
| id       | INTEGER | Primary key           |
| username | TEXT    | Unique username       |
| password | TEXT    | User password         |
| role     | TEXT    | User role (admin/user)|

## Development

This project was developed using:
- Visual Studio Code
- MinGW G++ compiler
- SQLite3 for database management



## Core Team Members
- AMEN TESHOME
- AMIR ABDULJELIL
- BETSEGAW TESFAYE
- BINIYAM KINFE
- DEGAGA DASTA 
- BINYAM YALEW GEDAM
