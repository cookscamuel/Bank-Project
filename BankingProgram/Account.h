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
protected:
    double balance;
    int accountNumber;
public:
    Account(int accountNumber,sqlite3 *dbHelper): accountNumber(accountNumber){       
        
        int dbStatus = sqlite3_open("bank_system.db", &dbHelper);

        sqlite3_stmt* stmt;

        const char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).

        std::string sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);

        if (dbStatus != SQLITE_OK) {
            std::cout << "There was an error opening the database:" << sqlite3_errmsg(dbHelper) << std::endl;
        }

        dbStatus = sqlite3_prepare_v2(dbHelper, sql.c_str(), -1, &stmt, &errorMessage);

        if (dbStatus != SQLITE_OK) {
            std::cout << "There was an error preparing the statement: " << errorMessage << std::endl;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            balance = sqlite3_column_double(stmt, 0);
        }else{
            std::cout << "Account not found." << std::endl;
            exit(1);
        }

        sqlite3_close(dbHelper);

       
    };
    ~Account() = default;

    void virtual withdraw(sqlite3 *dbHandler);
    void virtual deposit(sqlite3 *dbHandler);
    void virtual display();

    static double grabBalance(void *data, int numColumns, char **columnValues, char **columnNames){
        return std::stod(columnValues[0]);
    };
};

#endif
