// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// main.cpp
// The logic for the menu interface users will interact with.

#include "DerivedAccounts.h"
#include "AccountHolder.h"


#include <iostream>

/*
    Inserting Data:
        Users must have unique email addresses and passwords.
        Account types must be unique

*/


int main() {

    sqlite3 *dbHandler; // Pointer variable used to carry out database interactions.
    int dbStatus; // Variable used to track the status of recently executed queries. Successful connections are 0s.

    dbStatus = sqlite3_open("bank_system.db", &dbHandler); // Attempt to open a database called bank_system.db. If it doesn't exist, make it.

    if (dbStatus != 0) { // Check to see if the database connection failed.
        std::cout << "An error occurred while trying to access the database.\nExiting..." << std::endl;
        system("pause");
        exit(0); // Exit the program.
    }

    // Create a variable that will hold a query used to create tables for the database.
    const char *createTables = R"(

        CREATE TABLE IF NOT EXISTS account_types (
            type_id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT NOT NULL,
            type VARCHAR(50) UNIQUE NOT NULL
        );

        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
            email VARCHAR(100) UNIQUE NOT NULL,
            password VARCHAR(32) UNIQUE NOT NULL,
            name VARCHAR(50) NOT NULL,
            address VARCHAR(100) NOT NULL,
            phone VARCHAR(14) NOT NULL
        );

        CREATE TABLE IF NOT EXISTS active_accounts (
            account_number INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT NOT NULL,
            user_id INTEGER NOT NULL,
            type_id INTEGER NOT NULL,
            balance DOUBLE NOT NULL
        );

    )";

    char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
    dbStatus = sqlite3_exec(dbHandler, createTables, 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

    if (dbStatus != 0) {
        std::cout << "An error occured while creating tables: " << errorMessage << std::endl;
        exit(0);
    }


    // Adding sample users to the DB
    const char *populate1 = "INSERT INTO users (email, password, name, address, phone) VALUES "
                                "('sam@nscc.ca', 'sac00i2', 'Samuel Cook', '1 Infinite Loop, CA', '(902)688-1229'),"
                                "('albowlly@nscc.ca', 'albowlly1897', 'Al Bowlly', 'Overlook Hotel, USA', '(244)531-5673'),"
                                "('mikemyers@nscc.ca', 'halloweenFan', 'Michael Myers', '45 Lampkin Lane, Haddonfield', '(196)719-7831')";

    // Add the account types to the DB
    const char *populate2 = "INSERT INTO account_types (type) VALUES "
                            "('Chequings'),"
                            "('Savings'),"
                            "('Fixed-Deposit')";

    // Add some active accounts to the DB
    const char *populate3 = "INSERT INTO active_accounts (account_number, user_id, type_id, balance) VALUES "
                            "('1', '1', '1', '10436.46'),"
                            "('2', '1', '3', '2934.52'),"
                            "('3', '2', '1', '635672.47'),"
                            "('4', '2', '2', '7357.84'),"
                            "('5', '3', '1', '167.34'),"
                            "('6', '3', '2', '34.75'),"
                            "('7', '3', '3', '1978.31')";

    // Execute the populate queries
    sqlite3_exec(dbHandler, populate1, 0, 0, &errorMessage);
    sqlite3_exec(dbHandler, populate2, 0, 0, &errorMessage);
    sqlite3_exec(dbHandler, populate3, 0, 0, &errorMessage);

    system("cls");
    do {

        std::string selection;

        std::cout << " ============ THE BANK ============" << std::endl;
        std::cout << " 1 - Login" << std::endl;
        std::cout << " 2 - Register" << std::endl;
        std::cout << " 3 - Exit\n Please enter your selection: ";
        getline(std::cin, selection);

        if (selection == "1") {

            std::string email;
            std::string password;

            system("cls");
            std::cout << " ============= LOGIN ==============\n" << std::endl;
            std::cout << " Please enter your email: ";
            getline(std::cin, email);
            std::cout << " Please enter your password: ";
            getline(std::cin, password);

            /*
                login stuff
            */

            std::cout << " Press any key to continue... ";
            system("pause >nul");
            system("cls");
            
        }
        else if (selection == "2") {

            std::string email;
            std::string password;
            
            system("cls");
            std::cout << " ============ REGISTER ============\n" << std::endl;
            std::cout << " Please enter your full name: ";
            getline(std::cin, email);
            std::cout << " Please enter your full address: ";
            getline(std::cin, password);
            std::cout << " Please enter your phone number: ";
            getline(std::cin, email);
            std::cout << " Please enter your email: ";
            getline(std::cin, password);
            std::cout << " Please enter your password: ";
            getline(std::cin, password);

            /*
                register
            */

            std::cout << " Press any key to continue... ";
            system("pause >nul");
            system("cls");

        }
        else if (selection == "3") {
            system("cls");
            std::cout << " Exiting...\n" << std::endl;
            break;
        }
        else {
            system("cls");
        }

    } while (true);


    sqlite3_close(dbHandler);

    return 0;
}

