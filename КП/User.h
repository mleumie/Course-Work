#pragma once
#include "Roles.h"
#include "MediaFile.h"
#include <string>
#include <vector>
#include <memory>
#include <ostream>

class User {
protected:
    std::string login;
    Role role;
    double balance;
public:
    User(std::string l, double b);
    virtual ~User();
    Role getRole() const;
    std::string getLogin() const;
    double getBalance() const;
    void addBalance(double sum);
    bool spend(double sum);
    void showInfo();
    friend std::ostream& operator<<(std::ostream& os, const User& u);
};

class Buyer : public User {
    std::vector<std::shared_ptr<MediaFile>> purchases;
public:
    Buyer(std::string l, double b = 0);
    bool buy(std::shared_ptr<MediaFile> item);
    void showPurchases() const;
    void printReceipt() const;
    bool saveReceipt() const;
    bool saveReceipt(const std::string& filename) const;
};

class Seller : public User {
    std::vector<std::shared_ptr<MediaFile>> userFiles;
public:
    Seller(std::string l, double b = 0);
    ~Seller();
    std::shared_ptr<MediaFile> getFile(int idx);
    void addFile(std::shared_ptr<MediaFile> item);
    void showUserFiles() const;
    bool removeFile(int idx);
    size_t getFilesCount() const;
};

class Admin : public User {
public:
    Admin(std::string l, double b = 0);
    void removeFile(Seller* seller, int idx);
};