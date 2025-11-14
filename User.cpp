#include "User.h"
#include <iostream>

User::User(std::string l, std::string p, double b)
    : login(l), password(p), balance(b), role(Role::Buyer) {
}

User::~User() {}
Role User::getRole() const { return role; }
std::string User::getLogin() const { return login; }
double User::getBalance() const { return balance; }
void User::addBalance(double sum) { balance += sum; }
bool User::spend(double sum) {
    if (balance >= sum) {
        balance -= sum;
        return true;
    }
    return false;
}

void User::showInfo() {
    std::cout << "Логин: " << login << ", Роль: ";
    if (role == Role::Buyer) std::cout << "Покупатель";
    else if (role == Role::Seller) std::cout << "Продавец";
    else std::cout << "Админ";
    std::cout << ", Баланс: " << balance << std::endl;
}

// Buyer
Buyer::Buyer(std::string l, std::string p, double b) : User(l, p, b) { role = Role::Buyer; }
void Buyer::buy(MediaFile* item) {
    if (!item) return;
    if (spend(item->getPrice())) {
        purchases.push_back(item);
        std::cout << "Покупка успешна: " << item->getTitle() << std::endl;
    }
    else {
        std::cout << "Недостаточно средств для покупки: " << item->getTitle() << std::endl;
    }
}
void Buyer::showPurchases() const {
    std::cout << "Ваши покупки:" << std::endl;
    if (purchases.empty()) std::cout << "Нет покупок." << std::endl;
    for (auto item : purchases) item->printInfo();
}

// Seller
Seller::Seller(std::string l, std::string p, double b) : User(l, p, b) { role = Role::Seller; }
void Seller::addFile(MediaFile* item) {
    UserFiles.push_back(item);
    std::cout << "Файл добавлен в магазин: " << item->getTitle() << std::endl;
}
void Seller::showUserFiles() const {
    std::cout << "Ваши файлы:\n";
    if (UserFiles.empty()) std::cout << "Нет файлов.\n";
    for (auto item : UserFiles) item->printInfo();
}
MediaFile* Seller::getFile(int idx) {
    if (idx < 1 || idx > static_cast<int>(UserFiles.size())) return nullptr;
    return UserFiles[idx - 1];
}
bool Seller::removeFile(int idx) {
    if (idx < 1 || idx > static_cast<int>(UserFiles.size())) return false;
    auto it = UserFiles.begin() + (idx - 1);
    delete* it;
    UserFiles.erase(it);
    return true;
}

// Admin
Admin::Admin(std::string l, std::string p, double b) : User(l, p, b) { role = Role::Admin; }
void Admin::removeFile(Seller* seller, int idx) {
    if (seller && seller->removeFile(idx)) {
        std::cout << "Файл удалён администратором\n";
    }
    else {
        std::cout << "Нет такого файла у продавца\n";
    }
}