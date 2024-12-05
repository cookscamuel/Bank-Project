// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// DerivedAccounts.h
// Header file for classes that derive from the Account class. Checkings, Savings, Fixed Deposit signatures are here.

#ifndef DERIVED_ACCOUNTS_H
#define DERIVED_ACCOUNTS_H

#include "Account.h"

class SavingsAccount : public Account{
    protected:
        double interestRate;
    public:
        SavingsAccount(int accountNumber, sqlite3 *dbHandler): Account(accountNumber, dbHandler){};
        void withdraw(sqlite3 *dbHandler) override;
        void deposit(sqlite3 *dbHandler) override;
        void display() override;
        double calculateInterest();
};

class CheckingAccount : public Account{
    public:
        CheckingAccount(int accountNumber, sqlite3 *dbHandler): Account(accountNumber, dbHandler){

        };
        void withdraw(sqlite3 *dbHandler) override;
        void deposit(sqlite3 *dbHandler) override;
        void display() override;
        void transferFunds(sqlite3 *dbHandler);
};

class FixedDepositAccount : public Account{
    protected:
        double penalty = .075;
    public:
        FixedDepositAccount(int accountNumber, sqlite3 *dbHandler): Account(accountNumber, dbHandler){};
        void withdraw(sqlite3 *dbHandler) override; //this one has penalties
        void deposit(sqlite3 *dbHandler) override;
        double calculatePenalty();
        void display() override;
};


#endif