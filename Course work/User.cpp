#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <conio.h>

#include "User.h"
#include "Encryption.h"
#include "StringGen.h"
#include "Roles.h"
#include "Transactions.h"

std::ostream& operator<<(std::ostream& stream, User& usr) {
    if (!stream.bad() && !stream.fail()) {
        stream << usr.login << "\n";
        stream << usr.passwd_hash << "\n";
        stream << usr.salt << "\n";
        stream << static_cast<int>(usr.role) << "\n";
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, User& usr) {
    char temp[MAX_STRING_LENGTH];
    int amount = 0;
    role temp_role;

    if (!stream.bad() && !stream.fail()) {
        stream.getline(temp, MAX_STRING_LENGTH);
        usr.setLogin(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        usr.setHashedPassword(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        usr.setSalt(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);

        temp_role = (role)amount;

        usr.setRole(temp_role);
    }

    return stream;
}

bool User::operator<(const User& usr) {
    return this->login < usr.login;
}

bool User::operator>(const User& usr) {
    return this->login > usr.login;
}

bool User::operator==(const User& usr) {
    return this->login == usr.login;
}

User::User() {
    login = "NONE";
    salt = StringGen::generate();

    passwd_hash = salt;
    toHash(passwd_hash);

    role = role::user;
}

User::User(std::string login, std::string passwd, std::string salt) {
    this->login = login;
    this->salt = salt;
    this->setHash(passwd);

    role = role::user;
}

User::User(const User& user) {
    this->login = user.login;
    this->passwd_hash = user.passwd_hash;
    this->role = user.role;
    this->salt = user.salt;
}

std::string User::getLogin() const {
    return login;
}

void User::setLogin(std::string login) {
    this->login = login;
}

std::string User::getHash() {
    return passwd_hash;
}

void User::setHash(std::string passwd) {
    this->passwd_hash = salt + passwd;
    toHash(passwd_hash);
}

std::string User::getSalt() {
    return salt;
}

void User::setSalt(std::string salt) {
    this->salt = salt;
}

int User::getRole() {
    return static_cast<int>(this->role);
}

void User::setRole(enum class role role) {
    this->role = role;
}

void User::setHashedPassword(std::string hashed_passwd) {
    this->passwd_hash = hashed_passwd;
}

void User::toScreen() {
    std::cout << *this;
}

std::string User::changeLogin() {
    char ch{};
    bool check = false;
    std::string login;

    std::cout << "¬ведите новый логин: " << std::endl;

    ch = getchar();

    do {
        login += ch;

        ch = getchar();
    } while (ch != '\n' && ch != '\r');

    return login;
}

std::string User::changePassword() {
    char ch{};
    bool check = false;
    std::string passwd;

    std::cout << "¬ведите новый пароль:" << std::endl;

    do {
        ch = _getch();

        if (ch == '\b' && !passwd.empty())
        {
            passwd.erase(passwd.size() - 1);
            std::cout << "\b \b";
        }
        else if (ch != '\n' && ch != '\r') {
            std::cout << '*';
            passwd += ch;
        }
    } while (ch != '\n' && ch != '\r');

    std::cout << std::endl;

    return passwd;
}
