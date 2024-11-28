// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// Account.h
// Base class for Bank Account Types to derive from.

class Account
{
protected:
    double balance;
    int accountNumber;
public:
    Account(int accountNumber, double balance): accountNumber(accountNumber), balance(balance) {};
    ~Account() = default;

    void virtual withdraw();
    void virtual deposit();
    void virtual display();
};


