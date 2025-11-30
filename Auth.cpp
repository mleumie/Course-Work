#include "Auth.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <vector>

// замен на хеш??
std::string encrypt(std::string input) {
    std::vector<char> word(input.begin(), input.end());
    std::string range = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < (int)input.length(); i++) {
        for (int j = 0; j < (int)range.length(); j++) {
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
    users.clear();
    std::ifstream in(dbFile);
    if (!in) return;
    std::string login, encrypted, roleStr;
    while (std::getline(in, login)) {
        if (!std::getline(in, encrypted)) break;
        if (!std::getline(in, roleStr)) break;
        users.push_back({ login, encrypted, stringToRole(roleStr) });
    }
}  

void Auth::saveUsers() const {
    std::ofstream out(dbFile);
    for (const auto& user : users) {
        out << user.login << '\n' << user.encrypted << '\n' << roleToString(user.role) << '\n';
    }
}

bool Auth::registerUser(AuthRole role) {
    std::string login, password;
    std::cout << "Введите логин: ";
    std::cin >> login;
    for (const AuthUser& u : users) {
        if (u.login == login) {
            std::cout << "Пользователь с таким логином уже есть\n";
            return false;
        }
    }
    std::cout << "Введите пароль: ";
    std::cin >> password;

    std::string encrypted = encrypt(password);

    users.push_back({ login, encrypted, role });
    saveUsers();
    std::cout << "Пользователь зарегистрирован как " << roleToString(role) << std::endl;
    return true;
}

AuthUser* Auth::loginUser() {
    std::string login, password;
    std::cout << "Логин: ";
    std::cin >> login;
    std::cout << "Пароль: ";
    std::cin >> password;

    for (AuthUser& user : users) {
        if (user.login == login) {
            std::string encrypted = encrypt(password);
            if (encrypted == user.encrypted) {
                std::cout << "Вход выполнен как " << roleToString(user.role) << std::endl;
                return &user;
            }
            else {
                std::cout << "Неверный пароль\n";
                return nullptr;
            }
        }
    }
    std::cout << "Пользователь не найден\n";
    return nullptr;
}
