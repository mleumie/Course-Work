#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdexcept>

#include "User.h"
#include "MediaShop.h"
#include "MediaFile.h"

// Helper functions for input
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
        try {
            std::istringstream iss(s);
            int v;
            if (iss >> v) return v;
            throw std::invalid_argument("Неверный формат");
        }
        catch (const std::exception&) {
            std::cout << "Неверный ввод, введите целое число.\n";
        }
    }
}

static double readDouble(const std::string& prompt = "", double def = -1.0) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return def;
        try {
            std::istringstream iss(s);
            double v;
            if (iss >> v) return v;
            throw std::invalid_argument("Неверный формат");
        }
        catch (const std::exception&) {
            std::cout << "Неверный ввод, введите число.\n";
        }
    }
}

// Display all files in shop
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

// Display search/filter results
static void displayResults(const std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>>& results) {
    if (results.empty()) {
        std::cout << "Ничего не найдено.\n";
        return;
    }
    std::cout << "\nНайдено файлов: " << results.size() << "\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& [seller, file] = results[i];
        std::cout << "[" << (i + 1) << "] Продавец: " << seller->getLogin() 
                  << " | " << file->getType() << " | " << file->getTitle()
                  << " | Автор: " << file->getAuthor()
                  << " | Цена: " << file->getPrice() << "\n";
    }
}

// Buyer Menu
void buyerMenu(Buyer& user, MediaShop& shop) {
    while (true) {
        std::cout << "\n--- Меню покупателя (" << user.getLogin() << ") ---\n"
            << "1. Просмотреть все файлы\n"
            << "2. Поиск по названию/автору\n"
            << "3. Фильтр по типу (Audio/Video/Image)\n"
            << "4. Фильтр по цене\n"
            << "5. Сортировка по цене\n"
            << "6. Сортировка по названию\n"
            << "7. Купить файл\n"
            << "8. Мои покупки\n"
            << "9. Отчёт о покупках\n"
            << "0. Выйти в главное меню\n> ";
        int choice = readInt("", -1);
        
        if (choice == 0) break;
        
        if (choice == 1) {
            listAllFiles(shop);
        }
        else if (choice == 2) {
            std::string q = readLine("Введите текст для поиска: ");
            if (q.empty()) continue;
            auto results = shop.searchAllFiles(q);
            displayResults(results);
        }
        else if (choice == 3) {
            std::cout << "Выберите тип: 1-Audio, 2-Video, 3-Image\n";
            int t = readInt("> ", -1);
            std::string type;
            if (t == 1) type = "Audio";
            else if (t == 2) type = "Video";
            else if (t == 3) type = "Image";
            else { std::cout << "Неверный тип\n"; continue; }
            auto results = shop.filterAllByType(type);
            displayResults(results);
        }
        else if (choice == 4) {
            double minPrice = readDouble("Минимальная цена: ", 0.0);
            double maxPrice = readDouble("Максимальная цена: ", 999999.0);
            auto results = shop.filterAllByPriceRange(minPrice, maxPrice);
            displayResults(results);
        }
        else if (choice == 5) {
            std::cout << "1 - по возрастанию, 2 - по убыванию\n";
            int order = readInt("> ", 1);
            auto results = shop.getAllFilesSortedByPrice(order == 1);
            displayResults(results);
        }
        else if (choice == 6) {
            std::cout << "1 - по возрастанию (А-Я), 2 - по убыванию (Я-А)\n";
            int order = readInt("> ", 1);
            auto results = shop.getAllFilesSortedByTitle(order == 1);
            displayResults(results);
        }
        else if (choice == 7) {
            listAllFiles(shop);
            int sellerIdx = readInt("Выберите номер продавца (Enter для отмены): ", -1);
            if (sellerIdx < 1 || sellerIdx > static_cast<int>(shop.getSellers().size())) {
                std::cout << "Неверный номер продавца\n";
                continue;
            }
            auto s = shop.getSellers()[sellerIdx - 1];
            if (!s) { std::cout << "Продавец не найден\n"; continue; }
            int fileIdx = readInt("Выберите номер файла у продавца: ", -1);
            auto f = s->getFile(fileIdx);
            if (!f) { std::cout << "Файл не найден\n"; continue; }
            std::cout << "Вы выбрали: " << f->getTitle() << ", цена: " << f->getPrice() << "\n";
            std::cout << "Ваш баланс: " << user.getBalance() << "\n";
            std::string ans = readLine("Подтвердить покупку? (y/n): ");
            if (ans == "y" || ans == "Y" || ans == "д" || ans == "Д") {
                user.buy(f);
            }
            else {
                std::cout << "Покупка отменена\n";
            }
        }
        else if (choice == 8) {
            user.showPurchases();
        }
        else if (choice == 9) {
            std::string report = user.generateReport();
            std::cout << report;
            
            std::string saveAns = readLine("Сохранить отчёт в файл? (y/n): ");
            if (saveAns == "y" || saveAns == "Y" || saveAns == "д" || saveAns == "Д") {
                try {
                    std::string filename = "report_" + user.getLogin() + ".txt";
                    std::ofstream outFile(filename);
                    if (!outFile) {
                        throw std::runtime_error("Не удалось создать файл");
                    }
                    outFile << report;
                    outFile.close();
                    std::cout << "Отчёт сохранён в файл: " << filename << "\n";
                }
                catch (const std::exception& e) {
                    std::cerr << "Ошибка сохранения: " << e.what() << "\n";
                }
            }
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }
}

// Seller Menu
void sellerMenu(Seller& user, MediaShop& /*shop*/) {
    while (true) {
        std::cout << "\n--- Меню продавца (" << user.getLogin() << ") ---\n"
            << "1. Просмотреть мои файлы\n"
            << "2. Добавить файл\n"
            << "3. Редактировать файл\n"
            << "4. Удалить файл\n"
            << "5. Поиск в моих файлах\n"
            << "6. Фильтр по типу\n"
            << "7. Фильтр по цене\n"
            << "8. Сортировать по цене\n"
            << "9. Сортировать по названию\n"
            << "10. Сортировать по размеру\n"
            << "0. Выйти в главное меню\n> ";
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
            
            try {
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
            catch (const std::exception& e) {
                std::cerr << "Ошибка добавления файла: " << e.what() << "\n";
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
        else if (choice == 5) {
            std::string q = readLine("Введите текст для поиска: ");
            auto results = user.searchByTitle(q);
            if (results.empty()) {
                std::cout << "Ничего не найдено\n";
            } else {
                std::cout << "Найдено " << results.size() << " файлов:\n";
                for (const auto& f : results) {
                    if (f) f->printInfo();
                }
            }
        }
        else if (choice == 6) {
            std::cout << "Выберите тип: 1-Audio, 2-Video, 3-Image\n";
            int t = readInt("> ", -1);
            std::string type;
            if (t == 1) type = "Audio";
            else if (t == 2) type = "Video";
            else if (t == 3) type = "Image";
            else { std::cout << "Неверный тип\n"; continue; }
            auto results = user.filterByType(type);
            if (results.empty()) {
                std::cout << "Файлы не найдены\n";
            } else {
                std::cout << "Найдено " << results.size() << " файлов:\n";
                for (const auto& f : results) {
                    if (f) f->printInfo();
                }
            }
        }
        else if (choice == 7) {
            double minPrice = readDouble("Минимальная цена: ", 0.0);
            double maxPrice = readDouble("Максимальная цена: ", 999999.0);
            auto results = user.filterByPriceRange(minPrice, maxPrice);
            if (results.empty()) {
                std::cout << "Файлы не найдены\n";
            } else {
                std::cout << "Найдено " << results.size() << " файлов:\n";
                for (const auto& f : results) {
                    if (f) f->printInfo();
                }
            }
        }
        else if (choice == 8) {
            std::cout << "1 - по возрастанию, 2 - по убыванию\n";
            int order = readInt("> ", 1);
            user.sortFilesByPrice(order == 1);
            std::cout << "Файлы отсортированы по цене\n";
            user.showUserFiles();
        }
        else if (choice == 9) {
            std::cout << "1 - по возрастанию (А-Я), 2 - по убыванию (Я-А)\n";
            int order = readInt("> ", 1);
            user.sortFilesByTitle(order == 1);
            std::cout << "Файлы отсортированы по названию\n";
            user.showUserFiles();
        }
        else if (choice == 10) {
            std::cout << "1 - по возрастанию, 2 - по убыванию\n";
            int order = readInt("> ", 1);
            user.sortFilesBySize(order == 1);
            std::cout << "Файлы отсортированы по размеру\n";
            user.showUserFiles();
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }
}

// Admin Menu
void adminMenu(Admin& admin, MediaShop& shop) {
    while (true) {
        std::cout << "\n--- Меню админа (" << admin.getLogin() << ") ---\n"
            << "1. Показать продавцов\n"
            << "2. Показать файлы продавца\n"
            << "3. Удалить файл у продавца\n"
            << "4. Поиск по всем файлам\n"
            << "5. Статистика магазина\n"
            << "6. Отчёт по каталогу\n"
            << "0. Выйти в главное меню\n> ";
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
        else if (choice == 4) {
            std::string q = readLine("Введите текст для поиска: ");
            auto results = shop.searchAllFiles(q);
            displayResults(results);
        }
        else if (choice == 5) {
            std::string stats = Admin::generateShopStatistics();
            std::cout << stats;
        }
        else if (choice == 6) {
            std::string report = shop.generateCatalogReport();
            std::cout << report;
            
            std::string saveAns = readLine("Сохранить отчёт в файл? (y/n): ");
            if (saveAns == "y" || saveAns == "Y" || saveAns == "д" || saveAns == "Д") {
                try {
                    std::string filename = "catalog_report.txt";
                    std::ofstream outFile(filename);
                    if (!outFile) {
                        throw std::runtime_error("Не удалось создать файл");
                    }
                    outFile << report;
                    outFile.close();
                    std::cout << "Отчёт сохранён в файл: " << filename << "\n";
                }
                catch (const std::exception& e) {
                    std::cerr << "Ошибка сохранения: " << e.what() << "\n";
                }
            }
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }
}