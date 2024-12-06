// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// AccountHolder.h
// Class used to store login and personal information for bank customers.

#include <string>

#ifndef ACCOUNT_HOLDER_H
#define ACCOUNT_HOLDER_H

class AccountHolder
{
public:

    int id;
    std::string name;
    std::string address;
    std::string email;
    std::string password;
    std::string phone;
    
    ~AccountHolder() = default;
};

#endif