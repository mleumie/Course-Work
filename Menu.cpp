#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <string>
#include <iomanip>

#include "User.h"
#include "MediaShop.h"
#include "MediaFile.h"

// --- helpers (минимально, без дополнительных файлов) ---
static std::string readLine(const std::string& prompt = "") {
    std::string s;
    if (!prompt.empty()) std::cout << prompt;
    if (!std::getline(std::cin >> std::ws, s)) {
        std::cin.clear();
        return std::string();
    }
    return s;
}

static int readInt(const std::string& prompt = "", int def = -1) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return def;
        std::istringstream iss(s);
        int v;
        if (iss >> v) return v;
        std::cout << "Неверный ввод, введите целое число.\n";
    }
}

static double readDouble(const std::string& prompt = "", double def = -1.0) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return def;
        std::istringstream iss(s);
        double v;
        if (iss >> v) return v;
        std::cout << "Неверный ввод, введите число.\n";
    }
}

// Показать все файлы в магазине (коротко)
static void listAllFiles(const MediaShop& shop) {
    const auto& sellers = shop.getSellers();
    if (sellers.empty()) {
        std::cout << "В магазине нет продавцов.\n";
        return;
    }
    for (size_t i = 0; i < sellers.size(); ++i) {
        auto s = sellers[i];
        if (!s) continue;
        std::cout << "\n--- Продавец " << (i + 1) << ": " << s->getLogin()
            << " (баланс: " << s->getBalance() << ")\n";
        size_t cnt = s->getFilesCount();
        if (cnt == 0) {
            std::cout << "  (нет файлов)\n";
            continue;
        }
        for (size_t j = 1; j <= cnt; ++j) {
            auto f = s->getFile(static_cast<int>(j));
            if (!f) continue;
            std::cout << "  [" << j << "] " << f->getType() << " | " << f->getTitle()
                << " | Автор: " << f->getAuthor()
                << " | Цена: " << f->getPrice()
                << " | Размер: " << f->getSize() << " МБ\n";
        }
    }
}

// --- Меню покупателя ---
void buyerMenu(Buyer& user, MediaShop& shop) {
    while (true) {
        std::cout << "\n--- Меню покупателя (" << user.getLogin() << ") ---\n"
            << "1. Просмотреть все файлы\n"
            << "2. Поиск по названию\n"
            << "3. Купить файл\n"
            << "4. Мои покупки\n"
            << "0. Выход в главное меню\n> ";
        int choice = readInt("", -1);
        if (choice == 0) break;
        if (choice == 1) {
            listAllFiles(shop);
        }
        else if (choice == 2) {
            std::string q = readLine("Введите часть названия для поиска: ");
            if (q.empty()) continue;
            bool found = false;
            std::string ql = q;
            std::transform(ql.begin(), ql.end(), ql.begin(), ::tolower);
            for (auto& s : shop.getSellers()) {
                if (!s) continue;
                for (size_t j = 1; j <= s->getFilesCount(); ++j) {
                    auto f = s->getFile((int)j);
                    if (!f) continue;
                    std::string t = f->getTitle();
                    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
                    if (t.find(ql) != std::string::npos) {
                        std::cout << "Продавец: " << s->getLogin() << " [" << j << "] "
                            << f->getTitle() << " | Цена: " << f->getPrice() << "\n";
                        found = true;
                    }
                }
            }
            if (!found) std::cout << "Ничего не найдено\n";
        }
        else if (choice == 3) {
            listAllFiles(shop);
            int sellerIdx = readInt("Выберите номер продавца (Enter для отмены): ", -1);
            if (sellerIdx < 1 || sellerIdx > static_cast<int>(shop.getSellers().size())) {
                std::cout << "Отмена или неверный продавец\n";
                continue;
            }
            auto s = shop.getSellers()[sellerIdx - 1];
            if (!s) { std::cout << "Продавец не найден\n"; continue; }
            int fileIdx = readInt("Выберите номер файла у продавца: ", -1);
            auto f = s->getFile(fileIdx);
            if (!f) { std::cout << "Файл не найден\n"; continue; }
            std::cout << "Вы выбрали: " << f->getTitle() << ", цена: " << f->getPrice() << "\n";
            std::string ans = readLine("Подтвердить покупку? (y/n): ");
            if (ans == "y" || ans == "Y") {
                if (user.spend(f->getPrice())) {
                    user.buy(f);
                    std::cout << "Покупка успешна\n";
                }
                else {
                    std::cout << "Недостаточно средств\n";
                }
            }
            else {
                std::cout << "Покупка отменена\n";
            }
        }
        else if (choice == 4) {
            user.showPurchases();
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }
}

// --- Меню продавца ---
void sellerMenu(Seller& user, MediaShop& /*shop*/) {
    while (true) {
        std::cout << "\n--- Меню продавца (" << user.getLogin() << ") ---\n"
            << "1. Просмотреть мои файлы\n"
            << "2. Добавить файл\n"
            << "3. Редактировать файл\n"
            << "4. Удалить файл\n"
            << "0. Выход в главное меню\n> ";
        int choice = readInt("", -1);
        if (choice == 0) break;
        if (choice == 1) {
            user.showUserFiles();
        }
        else if (choice == 2) {
            std::cout << "Выберите тип файла: 1- Audio, 2- Video, 3- Image\n";
            int t = readInt("Тип: ", -1);
            std::string title = readLine("Название: ");
            std::string author = readLine("Автор: ");
            double price = readDouble("Цена: ", 0.0);
            int size = readInt("Размер (МБ): ", 0);
            std::string format = readLine("Формат (например mp3/jpg): ");
            if (t == 1) {
                int dur = readInt("Длительность (сек.): ", 0);
                std::string album = readLine("Альбом: ");
                user.addFile(std::make_shared<AudioFile>(title, author, price, size, format, dur, album));
            }
            else if (t == 2) {
                int dur = readInt("Длительность (сек.): ", 0);
                std::string res = readLine("Разрешение (например 1920x1080): ");
                user.addFile(std::make_shared<VideoFile>(title, author, price, size, format, dur, res));
            }
            else if (t == 3) {
                std::string res = readLine("Разрешение (например 800x600): ");
                user.addFile(std::make_shared<ImageFile>(title, author, price, size, format, res));
            }
            else {
                std::cout << "Неверный тип\n";
            }
        }
        else if (choice == 3) {
            user.showUserFiles();
            int idx = readInt("Номер файла для редактирования: ", -1);
            auto f = user.getFile(idx);
            if (!f) { std::cout << "Файл не найден\n"; continue; }
            f->edit();
            std::cout << "Файл отредактирован\n";
        }
        else if (choice == 4) {
            user.showUserFiles();
            int idx = readInt("Номер файла для удаления: ", -1);
            if (user.removeFile(idx)) std::cout << "Файл удалён\n";
            else std::cout << "Не найден файл с таким номером\n";
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }
}

// --- Меню админа ---
void adminMenu(Admin& admin, MediaShop& shop) {
    while (true) {
        std::cout << "\n--- Меню админа (" << admin.getLogin() << ") ---\n"
            << "1. Показать продавцов\n"
            << "2. Показать файлы продавца\n"
            << "3. Удалить файл у продавца\n"
            << "0. Выход в главное меню\n> ";
        int choice = readInt("", -1);
        if (choice == 0) break;
        if (choice == 1) {
            shop.showSellers();
        }
        else if (choice == 2) {
            shop.showSellers();
            int sidx = readInt("Номер продавца: ", -1);
            auto s = (sidx >= 1 && sidx <= static_cast<int>(shop.getSellers().size())) ? shop.getSellers()[sidx - 1] : nullptr;
            if (!s) { std::cout << "Продавец не найден\n"; continue; }
            s->showUserFiles();
        }
        else if (choice == 3) {
            shop.showSellers();
            int sidx = readInt("Номер продавца: ", -1);
            auto s = (sidx >= 1 && sidx <= static_cast<int>(shop.getSellers().size())) ? shop.getSellers()[sidx - 1] : nullptr;
            if (!s) { std::cout << "Продавец не найден\n"; continue; }
            s->showUserFiles();
            int fidx = readInt("Номер файла для удаления: ", -1);
            if (s->removeFile(fidx)) std::cout << "Файл удалён\n";
            else std::cout << "Не найден файл\n";
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }
}