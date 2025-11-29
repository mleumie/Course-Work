#include "User.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <stdexcept>

namespace MediaShopNS {

// Static member initialization
int User::totalUsersCount = 0;

User::User(std::string l, double b)
    : login(std::move(l)), role(Role::Buyer), balance(b) {
    ++totalUsersCount;
}

User::~User() {
    --totalUsersCount;
}

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

int User::getTotalUsersCount() { return totalUsersCount; }

void User::showInfo() {
    std::cout << "Логин: " << login << ", Роль: ";
    if (role == Role::Buyer) std::cout << "Покупатель";
    else if (role == Role::Seller) std::cout << "Продавец";
    else std::cout << "Админ";
    std::cout << ", Баланс: " << balance << std::endl;
}

// Friend operator<< overload for User
std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "Пользователь: " << user.login << ", Баланс: " << std::fixed << std::setprecision(2) << user.balance;
    return os;
}

// Buyer
Buyer::Buyer(std::string l, double b) : User(std::move(l), b) { role = Role::Buyer; }

void Buyer::buy(std::shared_ptr<MediaFile> item) {
    if (!item) return;
    if (spend(item->getPrice())) {
        purchases.push_back(item);
        MediaFile::addSale(item->getPrice());  // Track sale in statistics
        std::cout << "Куплен файл: " << item->getTitle() << std::endl;
    }
    else {
        std::cout << "Недостаточно средств для покупки: " << item->getTitle() << std::endl;
    }
}

void Buyer::showPurchases() const {
    std::cout << "Список покупок:" << std::endl;
    if (purchases.empty()) std::cout << "Нет покупок" << std::endl;
    for (const auto& item : purchases) if (item) item->printInfo();
}

const std::vector<std::shared_ptr<MediaFile>>& Buyer::getPurchases() const {
    return purchases;
}

std::string Buyer::generateReport() const {
    std::ostringstream report;
    
    // Get current time
    std::time_t now = std::time(nullptr);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    report << "========================================\n";
    report << "           ОТЧЁТ О ПОКУПКАХ            \n";
    report << "========================================\n";
    report << "Дата отчёта: " << timeStr << "\n";
    report << "Покупатель: " << login << "\n";
    report << "Текущий баланс: " << std::fixed << std::setprecision(2) << balance << " руб.\n";
    report << "----------------------------------------\n";
    
    if (purchases.empty()) {
        report << "Покупки отсутствуют.\n";
    } else {
        double total = 0.0;
        int num = 1;
        for (const auto& item : purchases) {
            if (item) {
                report << num++ << ". " << item->getTitle() 
                       << " (" << item->getType() << ")\n";
                report << "   Автор: " << item->getAuthor() << "\n";
                report << "   Цена: " << std::fixed << std::setprecision(2) 
                       << item->getPrice() << " руб.\n";
                total += item->getPrice();
            }
        }
        report << "----------------------------------------\n";
        report << "ИТОГО: " << std::fixed << std::setprecision(2) << total << " руб.\n";
        report << "Количество файлов: " << purchases.size() << "\n";
    }
    
    report << "========================================\n";
    
    return report.str();
}

// Seller
Seller::Seller(std::string l, double b) : User(std::move(l), b) { role = Role::Seller; }
Seller::~Seller() {
    userFiles.clear();
}
size_t Seller::getFilesCount() const {
    return userFiles.size();
}

void Seller::addFile(std::shared_ptr<MediaFile> item) {
    userFiles.push_back(item);
    std::cout << "Файл добавлен в каталог: " << item->getTitle() << std::endl;
}

void Seller::showUserFiles() const {
    std::cout << "Мои файлы:\n";
    if (userFiles.empty()) {
        std::cout << "Нет файлов.\n";
        return;
    }
    int num = 1;
    for (const auto& item : userFiles) {
        if (item) {
            std::cout << "[" << num++ << "] ";
            item->printInfo();
        }
    }
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

const std::vector<std::shared_ptr<MediaFile>>& Seller::getFiles() const {
    return userFiles;
}

std::vector<std::shared_ptr<MediaFile>> Seller::searchByTitle(const std::string& query) const {
    std::vector<std::shared_ptr<MediaFile>> results;
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);
    
    for (const auto& file : userFiles) {
        if (file) {
            std::string titleLower = file->getTitle();
            std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            if (titleLower.find(queryLower) != std::string::npos) {
                results.push_back(file);
            }
        }
    }
    return results;
}

std::vector<std::shared_ptr<MediaFile>> Seller::filterByType(const std::string& type) const {
    std::vector<std::shared_ptr<MediaFile>> results;
    for (const auto& file : userFiles) {
        if (file && file->getType() == type) {
            results.push_back(file);
        }
    }
    return results;
}

std::vector<std::shared_ptr<MediaFile>> Seller::filterByPriceRange(double minPrice, double maxPrice) const {
    std::vector<std::shared_ptr<MediaFile>> results;
    for (const auto& file : userFiles) {
        if (file && file->getPrice() >= minPrice && file->getPrice() <= maxPrice) {
            results.push_back(file);
        }
    }
    return results;
}

void Seller::sortFilesByPrice(bool ascending) {
    SortHelper<MediaFile>::sortByPrice(userFiles, ascending);
}

void Seller::sortFilesByTitle(bool ascending) {
    SortHelper<MediaFile>::sortByTitle(userFiles, ascending);
}

void Seller::sortFilesBySize(bool ascending) {
    SortHelper<MediaFile>::sortBySize(userFiles, ascending);
}

// Admin
Admin::Admin(std::string l, double b) : User(std::move(l), b) { role = Role::Admin; }

void Admin::removeFile(Seller* seller, int idx) {
    if (seller && seller->removeFile(idx)) {
        std::cout << "Файл удалён администратором\n";
    }
    else {
        std::cout << "Нет такого файла у продавца\n";
    }
}

std::string Admin::generateShopStatistics() {
    std::ostringstream report;
    
    std::time_t now = std::time(nullptr);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    report << "========================================\n";
    report << "       СТАТИСТИКА МАГАЗИНА             \n";
    report << "========================================\n";
    report << "Дата отчёта: " << timeStr << "\n";
    report << "----------------------------------------\n";
    report << "Всего файлов в системе: " << MediaFile::getTotalFilesCount() << "\n";
    report << "Общая сумма продаж: " << std::fixed << std::setprecision(2) 
           << MediaFile::getTotalSalesAmount() << " руб.\n";
    report << "Всего пользователей: " << User::getTotalUsersCount() << "\n";
    report << "========================================\n";
    
    return report.str();
}

} // namespace MediaShopNS