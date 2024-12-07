// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// DerivedAccounts.cpp
// All logic for classes that derive from Account.h

#include "DerivedAccounts.h"


//Withdraw function implementations 
void SavingsAccount::withdraw(sqlite3 *dbHandler){
    //amount to be withdrawn from the account
    double amount;

    do{//error checking loop for when the user puts an amount higher that the balance and allows them to exit when a negative number is put in
        do{ //error checking loop
                std::cout << "Enter the amount you would like to withdraw, or -1 to exit: ";
                std::cin >> amount;

                //checks if it failed ie checks for alphabetical/special characters here
                if (std::cin.fail()) {
                    std::cin.clear(); // clears the error flag on cin so that it can be used again
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an integer value. \n" << std::endl; // gives the user an error message and prompts them to enter a valid integer
                }else {
                    break;
                }
        }while(true);

        

        if (amount > balance || amount < 0){ //error message for when the user tries to withdraw more than they have
            std::cout << "Insufficient funds or invalid amount (negative), Enter a valid amount." << std::endl;
        }else{
            //updates balance value in database
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            

            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).

            int dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            //check for succesfull updating
            if (dbStatus != SQLITE_OK) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                std::cout << "Withdrawal successful." << std::endl;
                // updates the balance member variable with the updated balance from the database
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    break;
                }
            }
        }
        
    }while(amount > balance || amount != -1);
    display();// calls the display function
}

//functionally exactly the same as the savings account withdraw function
void CheckingAccount::withdraw(sqlite3 *dbHandler){
    double amount;
    
    do{//error checking for the amount to be withdrawn but checks for overdraft instead of total balance
        do{ 
                std::cout << "Enter the amount you would like to withdraw, or -1 to exit: ";
                std::cin >> amount;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an integer value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        
        // rather than checking for total balance, we check for overdraft
        if (balance-amount < balance*0.1 || amount < 0){
            std::cout << "Funds would go into overdraft or Invalid Amount (negative), Enter a valid amount." << std::endl;
        }else{
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).
           

            int dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            if (dbStatus != SQLITE_OK) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                std::cout << "Withdrawal successful." << std::endl;
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                    break;
                }
            }
        }
        
    }while(balance-amount < balance*0.1 || amount != -1 );
    display(); 
}

//functionally the same as the savings account withdraw function but with a penalty applied onto the balance when withdrawing
void FixedDepositAccount::withdraw(sqlite3 *dbHandler){
    double amount;
    

    do{
        do{ //inform the user of the fee that will be charged
                std::cout << "Enter the amount you would like to withdraw, a fee of 7.5% of your balance will be charged, or -1 to exit: ";
                std::cin >> amount;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
                    std::cout << "\n ERROR: Invalid entry! Please enter an integer value. \n" << std::endl;
                }else {
                    break;
                }
        }while(true);

        

        if (amount > balance || (amount + calculatePenalty()) > balance || amount < 0 ){
            std::cout << "Insufficient funds or invalid amount (negative), Enter a valid amount." << std::endl;
        }else{                                                                  // the total amount the will be subtracted from the balance with amount and the penalty
            std::string sql = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount + calculatePenalty()) + " WHERE account_number = " + std::to_string(accountNumber);
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).

            int dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
            

            if (dbStatus != SQLITE_OK) {
                std::cout << "There was an error udating balance: " << errorMessage << std::endl;
            }
            else {
                std::cout << "Withdrawal successful." << std::endl;
                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
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
//functionally similar to the withdraw functions but adds to the balance instead of subtracting
//the deposit functions are all the same 
void SavingsAccount::deposit(sqlite3 *dbHandler){
    double amount;
    do{ //error checking loop for invalid amounts
        do{ //error checking loop same as withdraw functions 
                std::cout << "Enter the amount you would like to deposit, or -1 to exit: ";
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
            // updating and increasing the balance
            std::string sql = "UPDATE active_accounts SET balance = balance + " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
            
            char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).

            int dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            // updating balance member variable
            if (dbStatus == SQLITE_OK){
                std::cout << "Deposit successful." << std::endl;

                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
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
    }while(amount != -1);
    display();
}

void CheckingAccount::deposit(sqlite3 *dbHandler){
    double amount;
    do{
        do{ 
                std::cout << "Enter the amount you would like to deposit, or -1 to exit: ";
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

            int dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            if (dbStatus == SQLITE_OK){
                std::cout << "Deposit successful." << std::endl;

                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
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
    }while(amount != -1);
    display();
}

void FixedDepositAccount::deposit(sqlite3 *dbHandler){
    double amount;
    do{
        do{ 
                std::cout << "Enter the amount you would like to deposit, or -1 to exit: ";
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

            int dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.

            if (dbStatus == SQLITE_OK){
                std::cout << "Deposit successful." << std::endl;

                sql = "SELECT balance FROM active_accounts WHERE account_number = " + std::to_string(accountNumber);
                sqlite3_stmt* stmt;

                dbStatus = sqlite3_prepare_v2(dbHandler, sql.c_str(), -1, &stmt, nullptr);

                if (dbStatus != SQLITE_OK){
                    std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
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
    }while(amount != -1);
    display();
}

//Display Function Implementations
// add menus for doing stuff with the accounts
void SavingsAccount::display(){ //displays the account number, balance, account type, and the interest that would be earned
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Balance: $" << balance << std::endl;
    std::cout << "Account Type: Savings" << std::endl;
    std::cout << "Interest Earnings: $" << calculateInterest() << " with a rate of " << interestRate*100 << "%" << std::endl;
}

void CheckingAccount::display(){ //displays the account number, balance, and account type
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Balance: $" << balance << std::endl;
    std::cout << "Account Type: Checking" << std::endl;
}

void FixedDepositAccount::display(){ //displays the account number, balance, account type, and the penalty that would be charged
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Balance: $" << balance << std::endl;
    std::cout << "Account Type: Fixed Deposit" << std::endl;
    std::cout << "Penalty: 7.5%" << std::endl;
    std::cout << "Penalty Amount: $" << calculatePenalty() << std::endl;
}

//Specific Function Implementions
double SavingsAccount::calculateInterest(){ //calculates the interest-rate based on the balance and returns the interest that would be earned
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

void CheckingAccount::transferFunds(sqlite3 *dbHandler){ // allows the user to transfer funds from the checking account to another account
    double amount;
    int accountNumber2;

    
    do{ 
        std::cout << "Enter the account number you would like to transfer funds to: ";
        std::cin >> accountNumber2;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
            std::cout << "\n ERROR: Invalid entry! Please enter an double value. \n" << std::endl;
        }else {
            break;
        }
    }while(true);

    
    
    
    do{ 
        std::cout << "Enter the amount you would like to transfer: ";
        std::cin >> amount;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out whatever is in the buffer.
            std::cout << "\n ERROR: Invalid entry! Please enter an double value. \n" << std::endl;
        }else {
            break;
        }
    }while(true);

    

    if (balance-amount < balance*0.1 || amount < 0){
        std::cout << "Funds would go into overdraft or Invalid Amount (negative)" << std::endl;
    }else{
        if (accountNumber == accountNumber2){
            std::cout << "You can't transfer to the same account." << std::endl;
            display();
            return;
        }
        
        //checking if the account to transfer to exists
        std::string sqlCount = "SELECT COUNT(*) FROM active_accounts WHERE account_number = " + std::to_string(accountNumber2);
        sqlite3_stmt* stmt;
        
        int dbStatus = sqlite3_prepare_v2(dbHandler, sqlCount.c_str(), -1, &stmt, nullptr);

        if (dbStatus != SQLITE_OK){
            std::cout << "There was an error preparing the statement: " << sqlite3_errmsg(dbHandler) << std::endl;
            exit(1);
        }

        dbStatus = sqlite3_step(stmt);


        //exits the function if the account to transfer to doesn't exist or the query didn't return any results
        if(dbStatus == SQLITE_ROW){
            if (sqlite3_column_int(stmt, 0) == 0){
                std::cout << "Account not found." << std::endl;
                display();
                return;
            }
        }else{
            std::cout << "No Data Found" << std::endl;
            display();
            return;
        }

        std::string sql = "UPDATE active_accounts SET balance = balance + " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber2);
        std::string sql2 = "UPDATE active_accounts SET balance = balance - " + std::to_string(amount) + " WHERE account_number = " + std::to_string(accountNumber);
        
        char *errorMessage = nullptr; // This is used to display the resulting error message (if there is an error).

        dbStatus = sqlite3_exec(dbHandler, sql.c_str(), 0, 0, &errorMessage); // this will return a 0 if the query executes successfully.
        
        if (dbStatus != SQLITE_OK) {
            std::cout << "There was an error udating balance: " << errorMessage << std::endl;
        }else{
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
                    exit(1);
                }

                dbStatus = sqlite3_step(stmt);

                if(dbStatus == SQLITE_ROW){
                    balance = sqlite3_column_double(stmt, 0);
                }else{
                    std::cout << "No Data Found" << std::endl;
                }
            }
        }
        
        
    }
    display();
}

//returns the penalty that will be charged
double FixedDepositAccount::calculatePenalty(){
    return (balance * penalty);
}

