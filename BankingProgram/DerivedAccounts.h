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
        SavingsAccount(int accountNumber, double balance, double interestRate): Account(accountNumber, balance), interestRate(interestRate){};
        void withdraw() override;
        void deposit() override;
        void display() override;
        void calculateInterest();
};

class CheckingAccount : public Account{
    public:
        CheckingAccount(int accountNumber, double balance): Account(accountNumber, balance){};
        void withdraw() override;
        void deposit() override;
        void display() override;
        void transferFunds();
};

class FixedDepositAccount : public Account{
    public:
        void withdraw() override; //this one has penalties
        void deposit() override;
        void display() override;
};


#endif