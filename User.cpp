#include "User.h"
#include <iostream>

User::User(std::string l, double b)
    : login(l),  balance(b), role(Role::Buyer) {
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
Buyer::Buyer(std::string l, double b) : User(l, b) { role = Role::Buyer; }
void Buyer::buy(std::shared_ptr<MediaFile> item) {
    if (!item) return;
    if (spend(item->getPrice())) {
        purchases.push_back(item);
        std::cout << "Куплен файл: " << item->getTitle() << std::endl;
    }
    else {
        std::cout << "Недостаточно средств для покупки: " << item->getTitle() << std::endl;
    }
}

void Buyer::showPurchases() const {
    std::cout << "Список покупок:" << std::endl;
    if (purchases.empty()) std::cout << "Нет покупок" << std::endl;
    for (auto item : purchases) if (item) item->printInfo();
}

// Seller
Seller::Seller(std::string l, double b) : User(l, b) { role = Role::Seller; }
Seller::~Seller() {
    userFiles.clear();
}
size_t Seller::getFilesCount() const {
    return userFiles.size();
}

void Seller::addFile(std::shared_ptr<MediaFile> item) {
    userFiles.push_back(item);
    std::cout << "Файл добавлен в магазин: " << item->getTitle() << std::endl;
}

void Seller::showUserFiles() const {
    std::cout << "Ваши файлы:\n";
    if (userFiles.empty()) std::cout << "Нет файлов.\n";
    for (auto item : userFiles) if (item) item->printInfo();
}

std::shared_ptr<MediaFile> Seller::getFile(int idx) {
    if (idx < 1 || idx > static_cast<int>(userFiles.size())) return nullptr;
    return userFiles[idx - 1];
}

bool Seller::removeFile(int idx) {
    if (idx < 1 || idx > static_cast<int>(userFiles.size())) return false;
    auto it = userFiles.begin() + (idx - 1);
    userFiles.erase(it);
    return true;
}

// Admin
Admin::Admin(std::string l, double b) : User(l, b) { role = Role::Admin; }
void Admin::removeFile(Seller* seller, int idx) {
    if (seller && seller->removeFile(idx)) {
        std::cout << "Файл удалён администратором\n";
    }
    else {
        std::cout << "Нет такого файла у продавца\n";
    }
}