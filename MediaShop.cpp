#include "MediaShop.h"
#include <iostream>

void MediaShop::addSeller(Seller* s) { sellers.push_back(s); }

std::vector<Seller*>& MediaShop::getSellers() { return sellers; }

void MediaShop::showSellers() const {
    std::cout << "Список продавцов:\n";
    for (size_t i = 0; i < sellers.size(); ++i)
        std::cout << i + 1 << ". " << sellers[i]->getLogin() << std::endl;
}
Seller* MediaShop::getSeller(int idx) {
    if (idx < 1 || idx > static_cast<int>(sellers.size())) return nullptr;
    return sellers[idx - 1];
}