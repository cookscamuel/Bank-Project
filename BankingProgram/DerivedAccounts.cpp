// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// DerivedAccounts.cpp
// All logic for classes that derive from Account.h

#include "DerivedAccounts.h"


//Withdraw function implementations
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

            if (dbStatus != SQLITE_OK) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    break;
                }
            }
        }
        
    }while(amount > balance || amount != -1);
    display();
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

            if (dbStatus != SQLITE_OK) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    break;
                }
            }
        }
        
    }while(amount > balance || amount != -1);
    display();
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
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount + calculatePenalty()) + " WHERE account_number = " + std::to_string(accountNumber);
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
            

            if (dbStatus != SQLITE_OK) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                   break;
                }
            }
        }
        
    }while(amount > balance || amount != -1);
    display();
}

//Deposit Function Implementations
void SavingsAccount::deposit(sqlite3 *dbHandler){
    double amount;
    do{
        do{ 
                std::cout << "Enter the amount you would like to deposit: ";
                std::cin >> amount;
                 if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an double value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        if (amount > 0){
            std::string sql = "UPDATE active_accounts SET balance = balance + " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            if (dbStatus == SQLITE_OK){
                std::cout << "Deposit successful." << std::endl;

                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                   break;
                }

            }else {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            

        }else if (amount == -1){
            break;
        }else{
            std::cout << "Invalid amount." << std::endl;
        }
    }while(amount > balance || amount != -1);
    display();
}

void CheckingAccount::deposit(sqlite3 *dbHandler){
    double amount;
    do{
        do{ 
                std::cout << "Enter the amount you would like to deposit: ";
                std::cin >> amount;
                 if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an double value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        if (amount > 0){
            std::string sql = "UPDATE active_accounts SET balance = balance + " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            if (dbStatus == SQLITE_OK){
                std::cout << "Deposit successful." << std::endl;

                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                   break;
                }

            }else {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            

        }else if (amount == -1){
            break;
        }else{
            std::cout << "Invalid amount." << std::endl;
        }
    }while(amount > balance || amount != -1);
    display();
}

void FixedDepositAccount::deposit(sqlite3 *dbHandler){
    double amount;
    do{
        do{ 
                std::cout << "Enter the amount you would like to deposit: ";
                std::cin >> amount;
                 if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an double value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        if (amount > 0){
            std::string sql = "UPDATE active_accounts SET balance = balance + " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           
            int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

            dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            if (dbStatus == SQLITE_OK){
                std::cout << "Deposit successful." << std::endl;

                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    sqlite3_close(dbHandler);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_int(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                   break;
                }

            }else {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            

        }else if (amount == -1){
            break;
        }else{
            std::cout << "Invalid amount." << std::endl;
        }
    }while(amount > balance || amount != -1);
    display();
}

//Display Function Implementations
// add menus for doing stuff with the accounts
void SavingsAccount::display(){
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Balance: " << balance << std::endl;
    std::cout << "Account Type: Savings" << std::endl;
    std::cout << "Interest Earnings: " << calculateInterest() << "with a rate of " << interestRate*100 << "%" << std::endl;
}

void CheckingAccount::display(){
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Balance: " << balance << std::endl;
    std::cout << "Account Type: Checking" << std::endl;
}

void FixedDepositAccount::display(){
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Balance: " << balance << std::endl;
    std::cout << "Account Type: Fixed Deposit" << std::endl;
    std::cout << "Penalty: 7.5%" << std::endl;
}

//Specific Function Implementions
double SavingsAccount::calculateInterest(){
    if (balance < 1000){
        interestRate = 0.01;
    }else if (balance < 5000){
        interestRate = 0.03;
    }else if (balance < 10000){
        interestRate = 0.05;
    }else{
        interestRate = 0.07;
    }
    return balance * interestRate;
    
}

void CheckingAccount::transferFunds(sqlite3 *dbHandler){
    double amount;
    int accountNumber;
    std::cout << "Enter the account number you would like to transfer funds to: ";
    std::cin >> accountNumber;
    std::cout << "Enter the amount you would like to transfer: ";
    std::cin >> amount;

    if (amount > balance){
        std::cout << "Insufficient funds." << std::endl;
    }else{
        std::string sql = "UPDATE active_accounts SET balance = balance + " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
        std::string sql2 = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
        
        char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
        int dbStatus = sqlite3_open("bank_system.db", &dbHandler);

        dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
        dbStatus = sqlite3_exec(dbHandler, sql2.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

        if (dbStatus != SQLITE_OK) {
            std::cout << "There was an error udating balance: " << errorMessage << std::endl;
        }
        else {
            sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
            sqlite3_stmt* stmt;

            dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

            if (dbStatus != SQLITE_OK){
                std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                sqlite3_close(dbHandler);
            }

            dbStatus = sqlite3_step(stmt);

            if(dbStatus == SQLITE_ROW){
                balance = sqlite3_column_int(stmt, 0);
            }else{
                std::cout << "No Data Found" << std::endl;
            }
        }
    }
    display();
}

double FixedDepositAccount::calculatePenalty(){
    return (balance * penalty);
}

