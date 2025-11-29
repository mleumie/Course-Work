#pragma once
#include <string>
#include <vector>
#include "MediaFile.h"
#include <memory>
#include <iostream>
#include <sstream>

namespace MediaShopNS {

enum class Role { Buyer, Seller, Admin };

// Forward declaration
class User;
std::ostream& operator<<(std::ostream& os, const User& user);

class User {
protected:
    std::string login;
    Role        role;
    double      balance;
    
    static int totalUsersCount;  // Static field to track total users
    
public:
    User(std::string l, double b);
    virtual ~User();
    Role        getRole() const;
    std::string getLogin() const;
    double      getBalance() const;
    void        addBalance(double sum);
    bool        spend(double sum);
    void        showInfo();
    
    // Static method
    static int getTotalUsersCount();
    
    // Friend function for operator overloading
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

class Buyer : public User {
    std::vector<std::shared_ptr<MediaFile>> purchases;
public:
    Buyer(std::string l, double b = 0);
    void buy(std::shared_ptr<MediaFile> item);
    void showPurchases() const;
    
    // Generate purchase report
    std::string generateReport() const;
    
    // Get purchases for iteration
    const std::vector<std::shared_ptr<MediaFile>>& getPurchases() const;
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
    
    // Get all files for searching/filtering
    const std::vector<std::shared_ptr<MediaFile>>& getFiles() const;
    
    // Search files by title (case-insensitive)
    std::vector<std::shared_ptr<MediaFile>> searchByTitle(const std::string& query) const;
    
    // Filter files by type
    std::vector<std::shared_ptr<MediaFile>> filterByType(const std::string& type) const;
    
    // Filter files by price range
    std::vector<std::shared_ptr<MediaFile>> filterByPriceRange(double minPrice, double maxPrice) const;
    
    // Sort files by price
    void sortFilesByPrice(bool ascending = true);
    
    // Sort files by title
    void sortFilesByTitle(bool ascending = true);
    
    // Sort files by size
    void sortFilesBySize(bool ascending = true);
};

class Admin : public User {
public:
    Admin(std::string l, double b = 0);
    void removeFile(Seller* seller, int idx);
    
    // Generate shop statistics report
    static std::string generateShopStatistics();
};

} // namespace MediaShopNS

// Bring types into global scope for compatibility
using MediaShopNS::Role;
using MediaShopNS::User;
using MediaShopNS::Buyer;
using MediaShopNS::Seller;
using MediaShopNS::Admin;