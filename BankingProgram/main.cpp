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
    AccountHolder* user = new AccountHolder();

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
            password VARCHAR(32) NOT NULL,
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


/////////////////////////////////////////////////////////// MAIN MENU STARTS HERE //////////////////////////////////////////////////////////////////////
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
            std::cout << " ============= LOGIN ==============" << std::endl;
            std::cout << " Please enter your email: ";
            getline(std::cin, email);
            std::cout << " Please enter your password: ";
            getline(std::cin, password);


            // Prepare a query to check for matches of the email and password.
            std::string sqlSelect = "SELECT COUNT(*) FROM users WHERE email = '" + email + "' AND password = '" + password + "';";
            sqlite3_stmt* stmt;

            dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
            if (dbStatus != SQLITE_OK) {
                std::cout << " There was an error signing you in." << std::endl;
            }

            int count = 0; // Variable used to count how many rows were returned.
            dbStatus = sqlite3_step(stmt); // Step through every row.
            if (dbStatus == SQLITE_ROW) {
                count = sqlite3_column_int(stmt, 0); // Set the count to the number of returned rows.
            }
            if (count == 0 || count > 1) { // If no records are found or, for some reason, duplicate records are found.
                std::cout << "\n We have no record of you at this bank.\n Please consider registering an account with us.\n" << std::endl;
                std::cout << " Press any key to continue...";
                system("pause >nul");
            }
            else {

                std::string sqlSelect = "SELECT user_id, email, password, name, address, phone FROM users WHERE email = '" + email + "' AND password = '" + password + "';";
                sqlite3_stmt* stmt;


                dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                if (dbStatus != SQLITE_OK) {
                    std::cout << " An error occurred while fetching your information." << std::endl;
                }

                dbStatus = sqlite3_step(stmt); // Step through every row.
                if (dbStatus == SQLITE_ROW) {
                    int user_id = sqlite3_column_int(stmt, 0);
                    const unsigned char* emailResult = sqlite3_column_text(stmt, 1);
                    const unsigned char* passwordResult = sqlite3_column_text(stmt, 2);
                    const unsigned char* name = sqlite3_column_text(stmt, 3);
                    const unsigned char* address = sqlite3_column_text(stmt, 4);
                    const unsigned char* phone = sqlite3_column_text(stmt, 5);

                    // Cast the query results into strings for the constructor to use.
                    std::string accEmail(reinterpret_cast<const char*>(emailResult));
                    std::string accPass(reinterpret_cast<const char*>(passwordResult));
                    std::string accName(reinterpret_cast<const char*>(name));
                    std::string accAddr(reinterpret_cast<const char*>(address));
                    std::string accPhone(reinterpret_cast<const char*>(phone));
                    
                    user->id = user_id;
                    user->email = accEmail;
                    user->password = accPass;
                    user->name = accName;
                    user->address = accAddr;
                    user->phone = accPhone;

                }

                sqlite3_finalize(stmt);
                system("cls");
                do {
                    selection = "";
                    std::cout << " ============ MY ACCOUNT ==========" << std::endl;
                    std::cout << " Hello, " << user->name << "." << std::endl;
                    std::cout << " 1 - View My Accounts" << std::endl;
                    std::cout << " 2 - Open New Account" << std::endl;
                    std::cout << " 3 - Manage Account" << std::endl;
                    std::cout << " 4 - Logout\n Please enter your selection: ";
                    getline(std::cin, selection);

                    if (selection == "1") {
                        system("cls");
                        std::cout << " Showing Accounts for " << user->name << "..." << std::endl;

                        std::string sqlSelect = "SELECT account_number, type_id, balance FROM active_accounts WHERE user_id = '" + std::to_string(user->id) + "';";
                        sqlite3_stmt* stmt;

                        /////////////////////////////////////////// HELPED BY CHATGPT ///////////////////////////////////////////////////
                        dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                        if (dbStatus != SQLITE_OK) {
                            std::cout << " There was an error processing your request." << std::endl;
                        }

                            std::string accType;
                            // Loop through the results and display them
                            while ((dbStatus = sqlite3_step(stmt)) == SQLITE_ROW) {
                                // Extract column values and display them
                                int account_number = sqlite3_column_int(stmt, 0);
                                int type_id = sqlite3_column_int(stmt, 1);
                                double balance = sqlite3_column_double(stmt, 2);

                                switch (type_id) {
                                    case 1:
                                        accType = "Chequings";
                                        break;
                                    case 2:
                                        accType = "Savings";
                                        break;

                                    case 3:
                                        accType = "Fixed-Deposit";
                                        break;
                                }
                                std::cout << "\n =========== ACCOUNT #" << account_number << " ===========" << std::endl;
                                std::cout << " " << accType << ", Balance: $" << balance << std::endl;
                            }

                            if (dbStatus != SQLITE_DONE) {
                                std::cout << " An error occurred while fetching your accounts." << std::endl;
                            }
                            sqlite3_finalize(stmt);

                        std::cout << "\n Press any key to return to the previous screen. ";
                        system("pause >nul");
                        system("cls");
                    }
                    else if (selection == "2") {
                        system("cls");
                        do {
                            selection = "";
                            std::cout << " ========= CREATE ACCOUNT =========" << std::endl;
                            std::cout << " 1 - New Chequing Account" << std::endl;
                            std::cout << " 2 - New Savings Account" << std::endl;
                            std::cout << " 3 - New Fixed-Deposit Account" << std::endl;
                            std::cout << " 4 - Back\n Please enter your selection: ";
                            getline(std::cin, selection);

                            if (selection == "1") {

                                std::string checkingQuery = "INSERT INTO active_accounts (user_id, type_id, balance) VALUES ('" + std::to_string(user->id) + "', 1, 0.00)";

                                dbStatus = sqlite3_exec(dbHandler, checkingQuery.c_str(), 0, 0, &errorMessage);

                                if (dbStatus != SQLITE_OK) {
                                    std::cout << " There was an error creating your account.\n Please try again." << errorMessage << std::endl;
                                }
                                else {
                                    std::cout << " New Chequing Account Opened!" << std::endl;
                                }

                                std::cout << " Press any key to continue...";
                                system("pause >nul");
                                
                                break;
                                
                            }
                            else if (selection == "2") {

                                std::string savingsQuery = "INSERT INTO active_accounts (user_id, type_id, balance) VALUES ('" + std::to_string(user->id) + "', 2, 0.00)";

                                dbStatus = sqlite3_exec(dbHandler, savingsQuery.c_str(), 0, 0, &errorMessage);

                                if (dbStatus != SQLITE_OK) {
                                    std::cout << " There was an error creating your account.\n Please try again." << errorMessage << std::endl;
                                }
                                else {
                                    std::cout << " New Savings Account Opened!" << std::endl;
                                }

                                std::cout << " Press any key to continue...";
                                system("pause >nul");

                                break;
                            }
                            else if (selection == "3") {

                                // MAKE NEW FIXED-DEPOSIT
                                std::string fixedQuery = "INSERT INTO active_accounts (user_id, type_id, balance) VALUES ('" + std::to_string(user->id) + "', 3, 0.00)";

                                dbStatus = sqlite3_exec(dbHandler, fixedQuery.c_str(), 0, 0, &errorMessage);

                                if (dbStatus != SQLITE_OK) {
                                    std::cout << " There was an error creating your account.\n Please try again." << errorMessage << std::endl;
                                }
                                else {
                                    std::cout << " New Fixed-Deposit Account Opened!" << std::endl;
                                }

                                std::cout << " Press any key to continue...";
                                system("pause >nul");

                                break;
                            }
                            else if (selection == "4") { // Return to My Account.
                                break;
                            }
                            else {
                                system("cls");
                            }
                        } while (true);
                        system("cls");

                    }
///////////////////////////////////////////////////////////////// MANAGE ACCOUNT //////////////////////////////////////////////////////////
                    else if (selection == "3") { // MANAGE ACCOUNT
                        do {
                            system("cls");
                            selection = "";
                            std::cout << " ========= MANAGE ACCOUNT =========" << std::endl;
                            std::cout << " Which account would you like to manage?" << std::endl;

                            std::string sqlSelect = "SELECT account_number, type_id, balance FROM active_accounts WHERE user_id = '" + std::to_string(user->id) + "';";
                            sqlite3_stmt* stmt;

                            /////////////////////////////////////////// HELPED BY CHATGPT ///////////////////////////////////////////////////
                            dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                            if (dbStatus != SQLITE_OK) {
                                std::cout << " There was an error processing your request." << std::endl;
                            }

                            std::string accType;
                            // Loop through the results and display them
                            while ((dbStatus = sqlite3_step(stmt)) == SQLITE_ROW) {
                                // Extract column values and display them
                                int account_number = sqlite3_column_int(stmt, 0);
                                int type_id = sqlite3_column_int(stmt, 1);
                                double balance = sqlite3_column_double(stmt, 2);

                                switch (type_id) {
                                    case 1:
                                        accType = "Chequings";
                                        break;
                                    case 2:
                                        accType = "Savings";
                                        break;

                                    case 3:
                                        accType = "Fixed-Deposit";
                                        break;
                                }
                                std::cout << "\n =========== ACCOUNT #" << account_number << " ===========" << std::endl;
                                std::cout << " " << accType << ", Balance: $" << balance << std::endl;
                            }

                            if (dbStatus != SQLITE_DONE) {
                                std::cout << " An error occurred while fetching your accounts." << std::endl;
                            }
                            sqlite3_finalize(stmt);
                            std::cout << "\n Please enter your selection, or\n enter 0 to return to My Account: ";
                            getline(std::cin, selection);

                            if (selection == "0") {
                                break;
                            }


                        } while (true);
                        system("cls");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else if (selection == "4") { // Logout, return to main menu.
                        break;
                    }
                    else { // Invalid input.
                        system("cls");
                    }

                } while (true);
            }
            system("cls");
            
        }
        else if (selection == "2") {

            std::string name;
            std::string address;
            std::string number;
            std::string email;
            std::string password;
            
            system("cls");
            std::cout << " ============ REGISTER ============" << std::endl;
            std::cout << " Please enter your full name: ";
            getline(std::cin, name);
            std::cout << " Please enter your full address: ";
            getline(std::cin, address);
            std::cout << " Please enter your phone number: ";
            getline(std::cin, number);
            std::cout << " Please enter your email: ";
            getline(std::cin, email);
            std::cout << " Please enter your password: ";
            getline(std::cin, password);

            // TODO: add some more error checking to ensure data entered is valid.

            // Query to insert new user into the database.
            std::string registerQuery = "INSERT INTO users (name, address, phone, email, password) VALUES ('" + name +"', '" + address + "', '" + number + "', '" + email + "', '" + password + "')";

            // Execute insert query.
            dbStatus = sqlite3_exec(dbHandler, registerQuery.c_str(), 0, 0, &errorMessage);

            if (dbStatus != SQLITE_OK) {
                std::cout << " There was an error creating your account.\n This email may already be in use." << std::endl;
            }
            else {
                std::cout << " Account created!" << std::endl;
            }

            std::cout << " Press any key to continue...";
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
    delete user;

    return 0;
}
