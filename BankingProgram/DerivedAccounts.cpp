// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// DerivedAccounts.cpp
// All logic for classes that derive from Account.h

#include "DerivedAccounts.h"

void SavingsAccount::withdraw(sqlite3 *dbHandler){
    double amount;
    

    do{
        do{ 
                std::cout << "Enter the amount you would like to withdraw: ";
                std::cin >> amount;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an integer value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        

        if (amount > balance){
            std::cout << "Insufficient funds." << std::endl;
        }else{
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
            // sqlite3_exec executes a query, it takes the database, the query, an error message, and some other shit as arguments.

            if (dbStatus != 0) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, &errorMessage);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << errorMessage << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    exit(1);
                }
            }
        }
        
    }while(amount > balance || amount == -1);
}

void CheckingAccount::withdraw(sqlite3 *dbHandler){
    double amount;
    

    do{
        do{ 
                std::cout << "Enter the amount you would like to withdraw: ";
                std::cin >> amount;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an integer value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        

        if (amount > balance){
            std::cout << "Insufficient funds." << std::endl;
        }else{
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
            // sqlite3_exec executes a query, it takes the database, the query, an error message, and some other shit as arguments.

            if (dbStatus != 0) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, &errorMessage);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << errorMessage << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    exit(1);
                }
            }
        }
        
    }while(amount > balance || amount == -1);
}

void FixedDepositAccount::withdraw(sqlite3 *dbHandler){
    double amount;
    

    do{
        do{ 
                std::cout << "Enter the amount you would like to withdraw, a fee of 7.5% of your balance will be charged: ";
                std::cin >> amount;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an integer value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        

        if (amount > balance){
            std::cout << "Insufficient funds." << std::endl;
        }else{
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount- (balance*0.075)) + " WHERE account_number = " + std::to_string(accountNumber);
            
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
            // sqlite3_exec executes a query, it takes the database, the query, an error message, and some other shit as arguments.

            if (dbStatus != 0) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, &errorMessage);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << errorMessage << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    exit(1);
                }
            }
        }
        
    }while(amount > balance || amount == -1);
}