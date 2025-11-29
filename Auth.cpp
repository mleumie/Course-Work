#include "Auth.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <vector>
#include <stdexcept>

// Caesar cipher encryption (shift by 3)
std::string encrypt(std::string input) {
    std::vector<char> word(input.begin(), input.end());
    std::string range = "abcdefghijklmnopqrstuvwxyz";

    for (size_t i = 0; i < input.length(); i++) {
        for (size_t j = 0; j < range.length(); j++) {
            if (word[i] == range[j]) {
                word[i] = range[(j + 3) % 26];
                break;
            }
        }
    }
    std::string str(word.begin(), word.end());
    return str;
}

Auth::Auth(const std::string& dbFile, const std::string& logFile)
    : dbFile(dbFile), logFile(logFile) {
    loadUsers();
}

void Auth::Authent(std::string& str) {
    str = encrypt(str);
}

std::string Auth::roleToString(AuthRole role) const {
    switch (role) {
    case AuthRole::buyer:  return "buyer";
    case AuthRole::seller: return "seller";
    case AuthRole::admin:  return "admin";
    default: return "buyer";
    }
}

AuthRole Auth::stringToRole(const std::string& str) {
    if (str == "buyer")  return AuthRole::buyer;
    if (str == "seller") return AuthRole::seller;
    if (str == "admin")  return AuthRole::admin;
    return AuthRole::buyer;
}

void Auth::loadUsers() {
    try {
        users.clear();
        std::ifstream in(dbFile);
        if (!in) return;  // File doesn't exist - not an error
        
        std::string login, encrypted, roleStr;
        while (std::getline(in, login)) {
            if (!std::getline(in, encrypted)) break;
            if (!std::getline(in, roleStr)) break;
            users.push_back({ login, encrypted, stringToRole(roleStr) });
        }
        
        if (in.bad()) {
            throw std::runtime_error("Error reading users file");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading users: " << e.what() << std::endl;
        users.clear();
    }
}  

void Auth::saveUsers() const {
    try {
        std::ofstream out(dbFile);
        if (!out) {
            throw std::runtime_error("Cannot open file for writing");
        }
        
        for (const auto& user : users) {
            out << user.login << '\n' << user.encrypted << '\n' << roleToString(user.role) << '\n';
            if (out.fail()) {
                throw std::runtime_error("Error writing user data");
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving users: " << e.what() << std::endl;
    }
}

bool Auth::registerUser(AuthRole role) {
    try {
        std::string login, password;
        std::cout << "Vvedite login: ";
        if (!(std::cin >> login)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw std::invalid_argument("Invalid login input");
        }
        
        for (const AuthUser& u : users) {
            if (u.login == login) {
                std::cout << "Polzovatel s takim loginom uzhe est\n";
                return false;
            }
        }
        
        std::cout << "Vvedite parol: ";
        if (!(std::cin >> password)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw std::invalid_argument("Invalid password input");
        }

        std::string encrypted = encrypt(password);

        users.push_back({ login, encrypted, role });
        saveUsers();
        std::cout << "Polzovatel zaregistrirovan kak " << roleToString(role) << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Registration error: " << e.what() << std::endl;
        return false;
    }
}

AuthUser* Auth::loginUser() {
    try {
        std::string login, password;
        std::cout << "Login: ";
        if (!(std::cin >> login)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw std::invalid_argument("Invalid login input");
        }
        
        std::cout << "Parol: ";
        if (!(std::cin >> password)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw std::invalid_argument("Invalid password input");
        }

        for (AuthUser& user : users) {
            if (user.login == login) {
                std::string encrypted = encrypt(password);
                if (encrypted == user.encrypted) {
                    std::cout << "Vhod vypolnen kak " << roleToString(user.role) << std::endl;
                    return &user;
                }
                else {
                    std::cout << "Nevernyj parol\n";
                    return nullptr;
                }
            }
        }
        std::cout << "Polzovatel ne najden\n";
        return nullptr;
    }
    catch (const std::exception& e) {
        std::cerr << "Login error: " << e.what() << std::endl;
        return nullptr;
    }
}
