#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

#include "Roles.h"
#include "BasicUser.h"

class User : public BasicUser {
private:
    std::string login;
    std::string passwd_hash;
    std::string salt;
    role role;

public:
    User();
    User(std::string, std::string, std::string);

    friend std::ostream& operator<<(std::ostream&, User&);
    friend std::istream& operator>>(std::istream&, User&);

    bool operator<(const User&);
    bool operator>(const User&);
    bool operator==(const User&);

    User(const User&);

    std::string getLogin() const;
    void setLogin(std::string);

    std::string getHash();
    void setHash(std::string);

    std::string getSalt();
    void setSalt(std::string);

    int getRole();
    void setRole(enum class role);

    void setHashedPassword(std::string);

    void toScreen();

    std::string changeLogin();
    std::string changePassword();
};

#endif
