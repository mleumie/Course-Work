#include "Auth.h"
#include "Roles.h"
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <limits>
#include <stdexcept>

static bool writeStr(const std::string& s, std::ofstream& out) {
    int len = static_cast<int>(s.size());
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (!out.good()) return false;
    if (len > 0) {
        out.write(s.data(), len);
        if (!out.good()) return false;
    }
    return true;
}
static bool readStr(std::string& s, std::ifstream& in) {
    int len = 0;
    if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
    if (len < 0) return false;
    s.clear();
    if (len == 0) return true;
    s.resize(static_cast<size_t>(len));
    if (!in.read(&s[0], len)) return false;
    return true;
}

Auth::Auth(const std::string& dbFile)
    : dbFile(dbFile) {
    loadUsers();
}

void Auth::loadUsers() {
    users.clear();
    std::ifstream in(dbFile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Не удалось открыть файл пользователей: " << dbFile << "\n";
        return;
    }
    int count = 0;
    if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
        if (in.bad()) {
            throw std::runtime_error("Критическая ошибка чтения количества пользователей из: " + dbFile);
        }
        std::cerr << "Не удалось прочитать количество пользователей из: " << dbFile << "\n";
        return;
    }
    if (count < 0) {
        throw std::runtime_error("Ошибка формата: отрицательное количество пользователей: " + dbFile);
    }
    for (int i = 0; i < count; i++) {
        std::string login, hash;
        if (!readStr(login, in)) {
            throw std::runtime_error("Не удалось прочитать логин пользователя #" + std::to_string(i) + " из " + dbFile);
        }
        if (!readStr(hash, in)) {
            throw std::runtime_error("Не удалось прочитать хеш пароля пользователя '" + login + "' из " + dbFile);
        }

        int roleCode = 0;
        if (!in.read(reinterpret_cast<char*>(&roleCode), sizeof(roleCode))) {
            throw std::runtime_error("Не удалось прочитать роль для пользователя '" + login + "' из " + dbFile);
        }
        Role role = Role::Buyer;
        if (roleCode == 1) role = Role::Seller;
        else if (roleCode == 2) role = Role::Admin;

        users.emplace(login, AuthUser{ login, hash, role });
    }
}

void Auth::saveUsers() {
    std::ofstream out(dbFile, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        throw std::runtime_error("Не удалось открыть файл пользователей для записи: " + dbFile);
    }

    int count = static_cast<int>(users.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    if (!out.good()) {
        out.close();
        throw std::runtime_error("Ошибка записи количества пользователей в: " + dbFile);
    }

    for (const auto& kv : users) {
        const AuthUser& u = kv.second;
        if (!writeStr(u.login, out)) {
            out.close();
            throw std::runtime_error("Не удалось записать логин: " + u.login);
        }
        if (!writeStr(u.passwordHash, out)) {
            out.close();
            throw std::runtime_error("Не удалось записать хеш пароля: " + u.login);
        }

        int roleCode = 0;
        if (u.role == Role::Seller) roleCode = 1;
        else if (u.role == Role::Admin) roleCode = 2;

        out.write(reinterpret_cast<const char*>(&roleCode), sizeof(roleCode));
        if (out.fail()) {
            out.close();
            throw std::runtime_error("Не удалось записать код роли: " + u.login);
        }
    }

    out.close();
    if (!out.good()) {
        std::cerr << "Предупреждение: поток записи имеет ошибки при закрытии файла пользователей\n";
    }
}

bool Auth::registerUser(Role role) {
    std::string login, password;
    std::cout << "Введите логин: ";
    std::cin >> login;
    if (users.find(login) != users.end()) {
        std::cout << "Пользователь уже существует\n";
        return false;
    }
    std::cout << "Введите пароль: ";
    std::cin >> password;

    std::random_device rd;
    std::string salt;
    for (int i = 0; i < 8; i++) salt.push_back(char('a' + (rd() % 26)));
    std::hash<std::string> hasher;
    auto hv = hasher(salt + password);
    std::ostringstream ss;
    ss << salt << ":" << std::hex << hv;
    std::string stored = ss.str();

    users.emplace(login, AuthUser{ login, stored, role });

    try {
        saveUsers();
    }
    catch (const std::exception& ex) {
        std::cerr << "Ошибка сохранения пользователей: " << ex.what() << "\n";
        return false;
    }

    std::cout << "Пользователь зарегистрирован\n";
    return true;
}

AuthUser* Auth::loginUser() {
    std::string login, password;
    std::cout << "Логин: ";
    std::cin >> login;
    std::cout << "Пароль: ";
    std::cin >> password;

    auto it = users.find(login);
    if (it == users.end()) {
        std::cout << "Нет такого пользователя\n";
        return nullptr;
    }
    AuthUser& u = it->second;
    auto pos = u.passwordHash.find(':');
    if (pos == std::string::npos) {
        std::cout << "Ошибка формата пароля\n";
        return nullptr;
    }
    std::string salt = u.passwordHash.substr(0, pos);
    std::hash<std::string> hasher;
    auto hv = hasher(salt + password);
    std::ostringstream hs;
    hs << std::hex << hv;
    if (hs.str() == u.passwordHash.substr(pos + 1)) {
        std::cout << "Вход выполнен\n";
        return &u;
    }
    std::cout << "Неверный пароль\n";
    return nullptr;
}