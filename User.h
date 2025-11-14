#pragma once
#include <string>
#include <vector>
#include "MediaFile.h"

enum class Role { Buyer, Seller, Admin };

class User {
protected:
    std::string login;
    std::string password;
    Role role;
    double balance;
public:
    User(std::string l, std::string p, double b);
    virtual ~User();
    Role getRole() const;
    std::string getLogin() const;
    double getBalance() const;
    void addBalance(double sum);
    bool spend(double sum);
    void showInfo();
};

class Buyer : public User {
    std::vector<MediaFile*> purchases;
public:
    Buyer(std::string l, std::string p, double b = 0);
    void buy(MediaFile* item);
    void showPurchases() const;
};

class Seller : public User {
    std::vector<MediaFile*> UserFiles;
public:
    Seller(std::string l, std::string p, double b = 0);
    void addFile(MediaFile* item);
    void showUserFiles() const;
    MediaFile* getFile(int idx);
    bool removeFile(int idx);
};

class Admin : public User {
public:
    Admin(std::string l, std::string p, double b = 0);
    void removeFile(Seller* seller, int idx);
};