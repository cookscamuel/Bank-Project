// Gabriel Ponce, Samuel Cook
// PROG2100
// 2024-11-14
// AccountHolder.h
#include <string>

#ifndef ACCOUNT_HOLDER_H
#define ACCOUNT_HOLDER_H

class AccountHolder
{
private:
    std::string name;
    std::string address;
    std::string email;
    std::string password;
    int phone;


public:
    AccountHolder(
        std::string name,
        std::string address,
        std::string email,
        std::string password,
        int phone
    ): name(name), address(address), email(email), password(password), phone(phone){};

    
    ~AccountHolder() = default;
};

#endif