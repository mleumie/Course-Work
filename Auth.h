#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

enum class AuthRole { buyer, seller, admin };

struct AuthUser {
    std::string login;
    std::string encrypted;
    AuthRole role;
};

class Auth {
public:
    Auth(const std::string& dbFile = "users.txt", const std::string& logFile = "login.log");
    void loadUsers();
    void saveUsers() const;

    bool registerUser(AuthRole role);
    AuthUser* loginUser();

private:
    std::vector<AuthUser> users;
    std::string dbFile;
    std::string logFile;

    static void Authent(std::string& str);
    static std::string roleToString(AuthRole role);
    static AuthRole stringToRole(const std::string& str);
};