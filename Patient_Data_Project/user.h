#ifndef USER_H
#define USER_H
#include <string>

class user
{
public:
    std::string name;
    std::string pw_hashed;


    user(std::string name, std::string pw_hashed);
    bool checkPW();
    void encrpypt_hashing();
    void decrypt_hashing();
};

#endif // USER_H
