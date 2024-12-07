// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// main.cpp
// The logic for the menu interface users will interact with.

#include "DerivedAccounts.h"
#include "AccountHolder.h"


#include <iostream>


int main() {

    sqlite3 *dbHandler; // Pointer variable used to carry out database interactions.
    int dbStatus; // Variable used to track the status of recently executed queries. Successful connections are 0s.
    AccountHolder* user = new AccountHolder(); // I made this at the start so it can reach all corners of the menu.
    // It's an AccountHolder object that will be used to hold logged-in user's data.
    // Every time a new user logs in, this object will have their data.
    // I thought it would be okay to make it here since the user has no way of accessing it before logging in.

    dbStatus = sqlite3_open("bank_system.db", &dbHandler); // Attempt to open a database called bank_system.db. If it doesn't exist, make it.

    if (dbStatus != 0) { // Check to see if the database connection failed.
        std::cout << "An error occurred while trying to access the database.\nExiting..." << std::endl;
        std::cout << " Press any key to continue...";
        system("pause >nul");
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

    if (dbStatus != 0) { // Check for any issues making the database.
        std::cout << "An error occured while creating tables: " << errorMessage << std::endl;
        exit(0);
    }


    // Adding sample users to the DB
    const char *populate1 = "INSERT INTO users (email, password, name, address, phone) VALUES "
                                "('sam@nscc.ca', 'sac00i2', 'Samuel Cook', '1 Infinite Loop, CA', '(902)688-1229'),"
                                "('albowlly@nscc.ca', 'albowlly1897', 'Al Bowlly', 'Overlook Hotel, USA', '(244)531-5673'),"
                                "('mikemyers@nscc.ca', 'halloweenFan', 'Michael Myers', '45 Lampkin Lane, Haddonfield', '(196)719-7831')";

    // Add the account types to the DB. This is a must!
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

    // populate1 and populate3 are not necessary, they could be removed entirely and there would be no accounts.

    // Main Menu starts here.
    system("cls"); // Clear the terminal, this happens a lot in this program.
    do { // Print the main menu for the user.

        std::string selection; // Used to hold the user's choice.

        std::cout << " ============ THE BANK ============" << std::endl;
        std::cout << " 1 - Login" << std::endl;
        std::cout << " 2 - Register" << std::endl;
        std::cout << " 3 - Exit\n Please enter your selection: ";
        getline(std::cin, selection);

        // Evaluate the user's selection.
        if (selection == "1") {

            // variables to hold login credentials.
            std::string email;
            std::string password;

            system("cls"); // Print the login screen.
            std::cout << " ============= LOGIN ==============" << std::endl;
            std::cout << " Please enter your email: ";
            getline(std::cin, email);
            std::cout << " Please enter your password: ";
            getline(std::cin, password);


            // Create a query to check for matches of the email and password.
            std::string sqlSelect = "SELECT COUNT(*) FROM users WHERE email = '" + email + "' AND password = '" + password + "';";
            sqlite3_stmt* stmt;

            // Prepare the query
            dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
            if (dbStatus != SQLITE_OK) { // Check if the query failed.
                std::cout << " There was an error signing you in." << std::endl;
            }

            int count = 0; // Variable used to count how many rows were returned.
            dbStatus = sqlite3_step(stmt); // Step through every row.
            if (dbStatus == SQLITE_ROW) {
                count = sqlite3_column_int(stmt, 0); // Set the count to the number of returned rows.
            }
            if (count == 0 || count > 1) { // If no records are found or, for some reason, duplicate records are found, inform the user of the failed login.
                std::cout << " Login Failed.\n We have no record of you at this bank.\n Please consider registering an account with us.\n" << std::endl;
                std::cout << " Press any key to continue...";
                system("pause >nul");
            }
            else {
                
                // Populate the user object upon login with the account information from the DB. 
                // It will be used to open accounts and display information without working with a specific account.
                std::string sqlSelect = "SELECT user_id, email, password, name, address, phone FROM users WHERE email = '" + email + "' AND password = '" + password + "';";
                sqlite3_stmt* stmt;

                // Check if the query failed for some reason.
                dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                if (dbStatus != SQLITE_OK) {
                    std::cout << " An error occurred while fetching your information." << std::endl;
                }

                dbStatus = sqlite3_step(stmt); // Step through every row.
                if (dbStatus == SQLITE_ROW) {
                    // Retrieve all of the queried values.
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

                    // I couldn't figure out how to avoid making a second set of variables.
                    
                    // Set the variables of the user object to the retrieved values of the most
                    // recently logged-in user. This will happen every time a new user logs in.

                    // We just made these variables public for simplicity's sake, but we could 
                    // have done the same thing with private variables by using setters.
                    user->id = user_id;
                    user->email = accEmail;
                    user->password = accPass;
                    user->name = accName;
                    user->address = accAddr;
                    user->phone = accPhone;

                }

                sqlite3_finalize(stmt); // Finalize the statement 
                system("cls");
                do { // Print the "Homepage" for logged in users.
                    selection = "";
                    std::cout << " ============ MY ACCOUNT ==========" << std::endl;
                    std::cout << " Hello, " << user->name << "." << std::endl;
                    std::cout << " 1 - View My Accounts" << std::endl;
                    std::cout << " 2 - Open New Account" << std::endl;
                    std::cout << " 3 - Manage Account" << std::endl;
                    std::cout << " 4 - Logout\n Please enter your selection: ";
                    getline(std::cin, selection);

                    if (selection == "1") { // Displaying all accounts tied to the logged-in user.
                        system("cls");
                        std::cout << " Showing Accounts for " << user->name << "..." << std::endl;

                        // Select all accounts based on the user's unique ID.
                        std::string sqlSelect = "SELECT account_number, type_id, balance FROM active_accounts WHERE user_id = '" + std::to_string(user->id) + "';";
                        sqlite3_stmt* stmt;

                        dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                        if (dbStatus != SQLITE_OK) {
                            std::cout << " There was an error processing your request." << std::endl;
                        }

                            std::string accType; // Used to hold a user-readable display of each account's type.
                            // Loop through the results.
                            while ((dbStatus = sqlite3_step(stmt)) == SQLITE_ROW) {
                                // Retrieve all queried values.
                                int account_number = sqlite3_column_int(stmt, 0);
                                int type_id = sqlite3_column_int(stmt, 1);
                                double balance = sqlite3_column_double(stmt, 2);

                                // Evaluate the type_id to display the corresponding account type.
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
                                // For each returned result, display the account type, the number, and the balance.
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
                    else if (selection == "2") { // Create a new account.
                        system("cls");
                        do { // Display the menu.
                            selection = "";
                            std::cout << " ========= CREATE ACCOUNT =========" << std::endl;
                            std::cout << " 1 - New Chequing Account" << std::endl;
                            std::cout << " 2 - New Savings Account" << std::endl;
                            std::cout << " 3 - New Fixed-Deposit Account" << std::endl;
                            std::cout << " 4 - Back\n Please enter your selection: ";
                            getline(std::cin, selection);

                            if (selection == "1") { // New Chequing Account

                                // Query to make a new active_account entry of chequing acount type, tied to the user's ID, with an empty balance.
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
                            else if (selection == "2") { // Make the same kind of entry in active_accounts, but for a savings account.

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
                            else if (selection == "3") { // ... and again, but for the Fixed-Deposit Account.

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
                            else if (selection == "4") { // Return to My Account with a simple break.
                                break;
                            }
                            else { // Invalid entry just clears the screen.
                                system("cls");
                            }
                        } while (true); // Loop forever.
                        system("cls"); // Clear the screen upon exiting the loop.

                    }
                    else if (selection == "3") { // Manage an existing account.
                        do { // Display all active accounts.
                            system("cls");
                            selection = "";
                            std::cout << " ========= MANAGE ACCOUNT =========" << std::endl;
                            std::cout << " Which account would you like to manage?" << std::endl;

                            // This is the same query as the "View My Accounts" feature. It retrieves all active accounts tied to the user.
                            std::string sqlSelect = "SELECT account_number, type_id, balance FROM active_accounts WHERE user_id = '" + std::to_string(user->id) + "';";
                            sqlite3_stmt* stmt;

                            dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                            if (dbStatus != SQLITE_OK) {
                                std::cout << " There was an error processing your request." << std::endl;
                            }

                            // This is doing the same thing that happens for View All Accounts, making the type meaningful.
                            std::string accType;
                            while ((dbStatus = sqlite3_step(stmt)) == SQLITE_ROW) {
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
                                // Display each account with the number, type, and balance.
                                std::cout << "\n =========== ACCOUNT #" << account_number << " ===========" << std::endl;
                                std::cout << " " << accType << ", Balance: $" << balance << std::endl;
                            }

                            if (dbStatus != SQLITE_DONE) {
                                std::cout << " An error occurred while fetching your accounts." << std::endl;
                            }
                            sqlite3_finalize(stmt);
                            
                            // Ask for input.
                            std::cout << "\n Please enter the account number, or\n enter 0 to return to My Account: ";
                            getline(std::cin, selection);

                            if (selection == "0") { // Check if the user wants to leave right away. 0 cannot be an account number, which is why it was selected.
                                break;
                            }
                            else {
                                // Retrieve the account number from the active account that the user selected. Guarantees the account exists.
                                std::string sqlSelect = "SELECT account_number, type_id FROM active_accounts WHERE user_id = '" + std::to_string(user->id) + "' AND account_number = '" + selection + "';";
                                sqlite3_stmt* stmt;

                                dbStatus = sqlite3_prepare_v2(dbHandler, sqlSelect.c_str(), -1, &stmt, nullptr);
                                if (dbStatus != SQLITE_OK) {
                                    std::cout << " An error occurred while selecting your account.\n Please try again later." << std::endl;
                                }

                                // Make sure a value was returned.
                                if ((dbStatus = sqlite3_step(stmt)) == SQLITE_ROW) {

                                    int account_number = sqlite3_column_int(stmt, 0);
                                    int type_id = sqlite3_column_int(stmt, 1);

                                    sqlite3_finalize(stmt);
                                    system("cls");

                                    if (type_id == 1) { // If the account the user picked was a chequing account...
                                        CheckingAccount chequings(account_number, dbHandler); // Make a stack object for the checking account using the account_number.
                                        do { // Display the menu.
                                            selection = "";
                                            chequings.display();
                                            std::cout << "\n What would you like to do?" << std::endl;
                                            std::cout << " 1 - Deposit" << std::endl;
                                            std::cout << " 2 - Withdraw" << std::endl;
                                            std::cout << " 3 - Transfer Funds" << std::endl;
                                            std::cout << " 4 - Back\n Please enter your selection: ";
                                            getline(std::cin, selection);

                                            // Evaluate the user's input.
                                            if (selection == "1") {
                                                chequings.deposit(dbHandler); // Calls the deposit function.
                                            }
                                            else if (selection == "2") {
                                                chequings.withdraw(dbHandler); // Calls the withdraw function.
                                            }
                                            else if (selection == "3") {
                                                chequings.transferFunds(dbHandler); // Calls the transfer funds function.
                                                system("pause"); // Let the user see the result of the transfer before clearing.
                                            }
                                            else if (selection == "4") { // Go to the previous menu.
                                                break;
                                            }
                                            else { // Invalid input clears the screen.
                                                system("cls");
                                            }

                                        } while (true);
                                    }
                                    else if (type_id == 2) { // If the account the user picked was a savings account...
                                        SavingsAccount savings(account_number, dbHandler); // Make a stack object for SavingsAccount.
                                        do { // Display the menu.
                                            selection = "";
                                            savings.display();
                                            std::cout << "\n What would you like to do?" << std::endl;
                                            std::cout << " 1 - Deposit" << std::endl;
                                            std::cout << " 2 - Withdraw" << std::endl;
                                            std::cout << " 3 - Back\n Please enter your selection: ";
                                            getline(std::cin, selection);

                                            if (selection == "1") { // Deposit
                                                savings.deposit(dbHandler);
                                            }
                                            else if (selection == "2") { // Withdraw.
                                                savings.withdraw(dbHandler);
                                            }
                                            else if (selection == "3") { // Go to the previous menu.
                                                break;
                                            }
                                            else { // Clear screen on invalid input.
                                                system("cls");
                                            }

                                        } while (true);
                                    }
                                    else if (type_id == 3) { // If the account the user picked was a chequing account...
                                        FixedDepositAccount fixed(account_number, dbHandler); // Make a new stack object for FixedDepositAccount.
                                        do { // Display the menu.
                                            selection = "";
                                            fixed.display();
                                            std::cout << "\n What would you like to do?" << std::endl;
                                            std::cout << " 1 - Deposit" << std::endl;
                                            std::cout << " 2 - Withdraw" << std::endl;
                                            std::cout << " 3 - Back\n Please enter your selection: ";
                                            getline(std::cin, selection);

                                            if (selection == "1") { // Deposit.
                                                fixed.deposit(dbHandler);
                                            }
                                            else if (selection == "2") { // Withdraw.
                                                fixed.withdraw(dbHandler);

                                            }
                                            else if (selection == "3") { // Go back.
                                                break;
                                            }
                                            else { // Invalid input, clear screen.
                                                system("cls");
                                            }

                                        } while (true);
                                    }
                                }
                            }
                        } while (true);
                        system("cls");
                    }
                    else if (selection == "4") { // Logout, return to main menu.
                        break;
                    }
                    else { // Invalid input, clear screen.
                        system("cls");
                    }

                } while (true);
            }
            system("cls");
            
        }
        else if (selection == "2") { // Register a new user-account for the bank. Does not make a Bank Account, just a login for a profile at the bank.

            // Variables to hold the user's entries.
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

            // I did not implement any data authentication since security was not one of
            // the main focuses of this project.

            // Query to insert new user into the database using their entries.
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
        else if (selection == "3") { // Exit the program.
            system("cls");
            std::cout << " Exiting...\n" << std::endl;
            break;
        }
        else {
            system("cls");
        }

    } while (true);


    sqlite3_close(dbHandler); // Close the dbHandler finally.
    delete user; // Delete the heap user object.

    return 0;
}