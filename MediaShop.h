#pragma once
#include <vector>
#include "User.h"

class MediaShop {
    std::vector<Seller*> sellers;
public:
    void addSeller(Seller* s);
    std::vector<Seller*>& getSellers();
    void showSellers() const;
    Seller* getSeller(int idx);
};
