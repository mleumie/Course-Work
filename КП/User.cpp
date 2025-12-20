#include "User.h"
#include "Table.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// --- User implementation ---

User::User(std::string l, double b)
    : login(l), balance(b), role(Role::Buyer) {
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
    std::cout << "Пользователь: " << login << ", роль: ";
    if (role == Role::Buyer) std::cout << "Покупатель";
    else if (role == Role::Seller) std::cout << "Продавец";
    else std::cout << "Админ";
    std::cout << ", баланс: " << balance << std::endl;
}

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << u.login << " (role=";
    switch (u.role) {
    case Role::Buyer: os << "Buyer"; break;
    case Role::Seller: os << "Seller"; break;
    case Role::Admin: os << "Admin"; break;
    }
    os << ", balance=" << std::fixed << std::setprecision(2) << u.balance << ")";
    return os;
}

// --- Buyer ---

Buyer::Buyer(std::string l, double b) : User(l, b) { role = Role::Buyer; }

bool Buyer::buy(std::shared_ptr<MediaFile> item) {
    if (!item) return false;
    if (spend(item->getPrice())) {
        purchases.push_back(item);
        std::cout << "Куплен файл: " << item->getTitle() << std::endl;
        return true;
    }
    else {
        std::cout << "Недостаточно средств для покупки: " << item->getTitle() << std::endl;
        return false;
    }
}

// Используем простую совместимую с Table::printFilesTable строку.
// Table::printFilesTable ожидает, что RowT имеет поля .file и .seller (seller может быть nullptr).
void Buyer::showPurchases() const {
    std::cout << "Мои покупки:\n";
    if (purchases.empty()) {
        std::cout << "(пока нет покупок)\n";
        return;
    }

    // Helper type that provides getLogin() for the table's Seller column.
    struct NameHolder {
        std::string name;
        std::string getLogin() const { return name; }
    };

    struct Row {
        std::shared_ptr<MediaFile> file;
        std::shared_ptr<NameHolder> seller; // nullptr for purchases
    };

    std::vector<Row> rows;
    rows.reserve(purchases.size());
    for (const auto& p : purchases) {
        rows.push_back(Row{ p, nullptr });
    }

    Table::printFilesTable(rows);
}

static void make_tm_from_time_t(std::time_t t, std::tm& out_tm) {
#if defined(_MSC_VER)
    localtime_s(&out_tm, &t);
#elif defined(__unix__) || defined(__APPLE__)
    localtime_r(&t, &out_tm);
#else
    std::tm* tmp = std::localtime(&t);
    if (tmp) out_tm = *tmp;
    else out_tm = std::tm();
#endif
}

// Печатает чек с информацией о покупках покупателя
void Buyer::printReceipt() const {
    if (purchases.empty()) { // Проверяем есть ли покупки
        std::cout << "Нет покупок\n"; // Сообщение об отсутствии покупок
        return;
    }
    auto now = std::chrono::system_clock::now(); // Получаем текущее время
    std::time_t t = std::chrono::system_clock::to_time_t(now); // Преобразуем в time_t
    std::tm tm; // Структура для хранения локального времени
    make_tm_from_time_t(t, tm); // Преобразуем time_t в tm локальное время
    std::cout << "\n===== Чек покупателя: " << login << " =====\n"; // Заголовок чека с логином
    std::cout << "Дата: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n\n"; // Выводим дату и время
    double total = 0.0; // Общая сумма покупок
    int idx = 1; // Счётчик позиций в чеке
    for (const auto& p : purchases) { // Перебираем все покупки
        if (!p) continue;
        std::cout << idx++ << ". " << p->getType() << " | " << p->getTitle() // Вывод типа и названия
            << " | Автор: " << p->getAuthor() // Автор
            << " | Цена: " << std::fixed << std::setprecision(2) << p->getPrice() // Цена
            << " | Размер: " << p->getSize() << " kB\n"; // Размер
        total += p->getPrice(); // Добавляем к общей сумме
    }
    std::cout << "\nИтого: " << std::fixed << std::setprecision(2) << total << " руб.\n"; // Вывод общей суммы
    std::cout << "=====================================\n\n"; // Нижняя граница чека
}

// Покупает указанный медиафайл (списывает средства и добавляет в список покупок)
bool Buyer::buy(std::shared_ptr<MediaFile> item) {
    if (!item) return false; // Проверяем файл
    if (spend(item->getPrice())) { // Пытаемся списать средства с баланса
        purchases.push_back(item); // Добавляем файл в список покупок
        std::cout << "Куплен файл: " << item->getTitle() << std::endl; // Сообщение об успешной покупке
        return true;
    }
    else { // Если средств недостаточно
        std::cout << "Недостаточно средств для покупки: " << item->getTitle() << std::endl; // Сообщение об ошибке
        return false;
    }
}

bool Buyer::saveReceipt(const std::string& filename) const {
    if (purchases.empty()) return false;

    std::string fname = filename;
    if (fname.empty()) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        make_tm_from_time_t(t, tm);
        std::ostringstream ss;
        ss << "receipt_" << login << "_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt";
        fname = ss.str();
    }

    std::ofstream out(fname);
    if (!out.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + fname);
    }

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    make_tm_from_time_t(t, tm);

    out << "Чек покупателя: " << login << "\n";
    out << "Дата: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n\n";
    if (out.fail()) {
        out.close();
        throw std::runtime_error("Ошибка записи в файл: " + fname);
    }

    double total = 0.0;
    int idx = 1;
    for (const auto& p : purchases) {
        if (!p) continue;
        out << idx++ << ". " << p->getType() << " | " << p->getTitle()
            << " | Автор: " << p->getAuthor()
            << " | Цена: " << std::fixed << std::setprecision(2) << p->getPrice()
            << " | Размер: " << p->getSize() << " kB\n";
        if (out.fail()) {
            out.close();
            throw std::runtime_error("Ошибка записи в файл: " + fname);
        }
        total += p->getPrice();
    }
    out << "\nИтого: " << std::fixed << std::setprecision(2) << total << " руб.\n";
    out.close();
    if (!out.good()) {
        throw std::runtime_error("Ошибка при закрытии файла: " + fname);
    }
    return true;
}

bool Buyer::saveReceipt() const {
    try {
        return saveReceipt(std::string());
    }
    catch (const std::exception& ex) {
        std::cerr << "Ошибка сохранения чека: " << ex.what() << "\n";
        return false;
    }
}

// --- Seller ---

Seller::Seller(std::string l, double b) : User(l, b) { role = Role::Seller; }
Seller::~Seller() { userFiles.clear(); }
size_t Seller::getFilesCount() const { return userFiles.size(); }
void Seller::addFile(std::shared_ptr<MediaFile> item) { userFiles.push_back(item); }

// Show seller files using Table::printFilesTable via small Row wrapper.
// Use the same NameHolder type so table's seller column works.
void Seller::showUserFiles() const {
    std::cout << "Файлы продавца: " << login << "\n";
    if (userFiles.empty()) {
        std::cout << "(нет файлов)\n";
        return;
    }

    struct NameHolder {
        std::string name;
        std::string getLogin() const { return name; }
    };
    struct Row {
        std::shared_ptr<MediaFile> file;
        std::shared_ptr<NameHolder> seller;
    };

    std::vector<Row> rows;
    rows.reserve(userFiles.size());
    for (const auto& f : userFiles) {
        auto nh = std::make_shared<NameHolder>();
        nh->name = login;
        rows.push_back(Row{ f, nh });
    }

    Table::printFilesTable(rows);
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

// --- Admin ---

Admin::Admin(std::string l, double b) : User(l, b) { role = Role::Admin; }
void Admin::removeFile(Seller* seller, int idx) {
    if (seller && seller->removeFile(idx)) {
        std::cout << "Файл удалён\n";
    }
    else std::cout << "Не удалось удалить файл\n";
}