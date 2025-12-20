#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <unordered_map>
#include "Roles.h"

struct AuthUser {
    std::string login;
	std::string passwordHash; //пароль в зашифрованном виде в формате соль:хеш
    Role role;
};

class Auth {
public:
    Auth(const std::string& dbFile = "users.dat");
    void loadUsers();
    void saveUsers();
    bool registerUser(Role role);
    AuthUser* loginUser();

private:
    std::unordered_map<std::string, AuthUser> users; // ключ = логин
    std::string dbFile;
    std::string logFile;
};