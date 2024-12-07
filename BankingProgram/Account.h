// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// Account.h
// Base class for Bank Account Types to derive from.

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <sqlite3.h>
#include <iostream>
#include <cmath>

class Account
{
    //protected so that the derived classes can access the members
protected:
    // will hold the balance of the account and the account number
    double balance;
    int accountNumber;
public:

    // Constructor for the Account class that grabs the balance from the database
    Account(int aN,sqlite3 *dbHelper): accountNumber(aN){       
        
        int dbStatus = sqlite3_open("bank_system.db", &dbHelper);

        sqlite3_stmt* stmt; // variable that holds the sql statement results

        const char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).

        //grabbing balance from the database based on the passed in account number
        std::string sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(aN);

        // checking if the database opened correctly
        if (dbStatus != SQLITE_OK) {
            std::cout << "There was an error opening the database:" << sqlite3_errmsg(dbHelper) << std::endl;
        }

        //preparing the query
        dbStatus = sqlite3_prepare_v2(dbHelper, sql.c_str(), -1, &stmt, &errorMessage);

        //checking if the query was prepared correctly
        if (dbStatus != SQLITE_OK) {
            std::cout << "There was an error preparing the statement: " << errorMessage << std::endl;
        }

        //executing the query and grabbing the balance which would be in the first column
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            balance = sqlite3_column_double(stmt, 0);
        }else{
            std::cout << "Account not found." << std::endl;
            //closes the program if it fails to find the account
            //this shouldn't happen because the accounts have 
            //already been found in main.cpp when looking for all
            //the accounts associated with the user
            exit(1);
        }
        sqlite3_finalize(stmt); // finalizing the statement
        sqlite3_close(dbHelper);

       
    };
    ~Account() = default;

    // virtual functions that will be overridden by the derived classes but will do very similar things
    virtual void withdraw(sqlite3 *dbHandler) = 0; // subtracts from the balance and updates the database and then updates the variable
    virtual void deposit(sqlite3 *dbHandler) = 0;  // adds to the balance and updates the database and then updates the variable
    virtual void display() = 0; //displays the account number and the balance
};

#endif