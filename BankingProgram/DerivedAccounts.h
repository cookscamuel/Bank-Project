// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// DerivedAccounts.h
// Header file for classes that derive from the Account class. Checkings, Savings, Fixed Deposit signatures are here.

#ifndef DERIVED_ACCOUNTS_H
#define DERIVED_ACCOUNTS_H

#include "Account.h"

//all of the accounts sub classes use the constructor from the base class

//savings account derived from account
class SavingsAccount : public Account{
    private:
        double interestRate;
    public:
        SavingsAccount(int accountNumber, sqlite3 *dbHandler): Account(accountNumber, dbHandler){};
        void withdraw(sqlite3 *dbHandler) override;
        void deposit(sqlite3 *dbHandler) override;
        void display() override;
        //calculate interest based on balance and display how much interest would be earned
        double calculateInterest();
};

//checking account derived from account
class CheckingAccount : public Account{
    //no specific private member functions needed
    public:
        CheckingAccount(int accountNumber, sqlite3 *dbHandler): Account(accountNumber, dbHandler){

        };
        void withdraw(sqlite3 *dbHandler) override;
        void deposit(sqlite3 *dbHandler) override;
        void display() override;
        //will allow the user to transfer funds to another account from this account
        void transferFunds(sqlite3 *dbHandler);
};


class FixedDepositAccount : public Account{
    private:
        // the penalty that will be applied if the user withdraws from the account
        double penalty = .075;
    public:
        FixedDepositAccount(int accountNumber, sqlite3 *dbHandler): Account(accountNumber, dbHandler){};
        void withdraw(sqlite3 *dbHandler) override; //this one has penalties
        void deposit(sqlite3 *dbHandler) override;
        double calculatePenalty(); // function used to calculate the penalty on the balance and is used within the withdraw function
        void display() override;
};


#endif