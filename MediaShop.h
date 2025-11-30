#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"

class MediaShop {
    std::vector<std::shared_ptr<Seller>> sellers;
public:
    void addSeller(const std::shared_ptr<Seller>& s);
    std::vector<std::shared_ptr<Seller>>& getSellers();
    const std::vector<std::shared_ptr<Seller>>& getSellers() const;

    void showSellers() const;
    std::shared_ptr<Seller> getSeller(int idx) const;
    std::shared_ptr<Seller> findSeller(const std::string& login) const;

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};