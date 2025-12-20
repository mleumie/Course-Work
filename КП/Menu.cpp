#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <string>
#include <iomanip>
#include <vector>
#include <conio.h>

#include "ConsoleUI.h"   // единая цветная реализация меню
#include "Table.h"
#include "User.h"
#include "MediaShop.h"
#include "MediaFile.h"

// Утилиты ввода
static std::string readLine(const std::string& prompt = "") {
    std::string s;
    if (!prompt.empty()) std::cout << prompt;
    if (!std::getline(std::cin >> std::ws, s)) { std::cin.clear(); return std::string(); }
    return s;
}
static int readInt(const std::string& prompt = "", int def = -1) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return def;
        std::istringstream iss(s);
        int v;
        if (iss >> v) return v;
        std::cout << "Неверный ввод, введите целое число\n";
    }
}
static double readDouble(const std::string& prompt = "", double def = -1.0) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return def;
        std::istringstream iss(s);
        double v;
        if (iss >> v) return v;
        std::cout << "Неверный ввод, введите число\n";
    }
}

// FileView для списка файлов
struct FileView {
    std::shared_ptr<Seller> seller;
    std::shared_ptr<MediaFile> file;
    int sellerIdx;
    int fileIdx;
};

// Собирает все файлы из магазина в единый вектор для отображения
static std::vector<FileView> collectAllFiles(const MediaShop& shop) {
    std::vector<FileView> out; // Вектор для хранения всех файлов
    const auto& sellers = shop.getSellers(); // Получаем список всех продавцов
    for (size_t si = 0; si < sellers.size(); si++) { // Перебираем продавцов
        auto s = sellers[si]; // Получаем текущего продавца
        if (!s) continue; // Пропускаем если продавец невалиден
        for (size_t fi = 1; fi <= s->getFilesCount(); fi++) { // Перебираем файлы продавца
            auto f = s->getFile(static_cast<int>(fi)); // Получаем файл по индексу
            if (!f) continue; // Пропускаем если файл невалиден
            // Добавляем FileView с информацией о продавце, файле и их индексах
            out.push_back(FileView{ s, f, static_cast<int>(si + 1), static_cast<int>(fi) });
        }
    }
    return out; // Возвращаем собранный вектор
}

// Показать все файлы подробно
static void showMediaFiles(const MediaShop& shop) {
    auto all = collectAllFiles(shop);
    if (all.empty()) {
        std::cout << "В магазине нет доступных файлов.\n";
        getInput();
        return;
    }
    // Table::printFilesTable ожидает, что элементы имеют .file и .seller — FileView это удовлетворяет.
    Table::printFilesTable(all);
    getInput();
}

// Выбор файла из списка (цветное меню)
static int chooseFileInteractive(const std::vector<FileView>& files, const std::string& title) {
    if (files.empty()) return -1;
    std::vector<std::string> items;
    items.reserve(files.size());
    for (const auto& fv : files) {
        std::ostringstream ss;
        ss << fv.file->getType() << " | " << fv.file->getTitle() << " | " << fv.file->getAuthor()
            << " | " << fv.file->getPrice() << " | " << fv.file->getSize() << "kB"
            << " | " << (fv.seller ? fv.seller->getLogin() : std::string("(none)"));
        items.push_back(ss.str());
    }
    return MenuStrings(items, title);
}

void buyerMenu(Buyer& user, MediaShop& shop) {
    while (true) {
        int choice = MenuStrings({
            "Показать все файлы",
            "Поиск по названию",
            "Фильтр по типу",
            "Сортировка",
            "Купить файл",
            "Мои покупки",
            "Печать/сохранение чека",
            "Баланс",
            "Пополнить баланс",
            "Назад"
            }, std::string("Меню покупателя: ") + user.getLogin()); // Формирует заголовок меню с логином пользователя

        if (choice < 0 || choice == 9) break; // Если нажат ESC или выбран пункт "Назад", выходим из меню

        switch (choice) { // Обработка выбранного пункта меню
        case 0:
			showMediaFiles(shop); // Показать все файлы магазина
            break;
        case 1: { // Поиск по названию
            std::string q = readLine("Введите название: "); // Запрос строки для поиска
            if (q.empty()) break; // Если строка пустая, выходим
            auto all = collectAllFiles(shop); // Получаем все файлы магазина
            std::vector<FileView> res; // Вектор для результатов поиска
            for (auto& f : all) if (f.file->getTitle().find(q) != std::string::npos) res.push_back(f); // Поиск по вхождению подстроки
            if (res.empty()) { std::cout << "Ничего не найдено\n"; getInput(); break; } // Сообщение при отсутствии результатов
            Table::printFilesTable(res); getInput(); break; // Вывод таблицы с результатами
        }
        case 2: { // Фильтр по типу
            int t = MenuStrings({ "Audio", "Video", "Image", "Отмена" }, "Выберите тип"); // Меню выбора типа файла
            if (t < 0 || t == 3) break; // Отмена выбора
            std::string type = (t == 0 ? "Audio" : (t == 1 ? "Video" : "Image")); // Преобразование выбора в строку
            auto all = collectAllFiles(shop); // Получение всех файлов
            std::vector<FileView> res; // Вектор для отфильтрованных файлов
            for (auto& f : all) if (f.file->getType() == type) res.push_back(f); // Фильтрация по типу
            Table::printFilesTable(res); getInput(); break; // Вывод результатов
        }
        case 3: { // Сортировка файлов
            int s = MenuStrings({ "Цена по возрастанию", "Цена по убыванию", "Название", "Отмена" }, "Сортировка"); // Меню выбора сортировки
            if (s < 0 || s == 3) break; // Отмена сортировки
            auto all = collectAllFiles(shop); // Получение всех файлов
            if (s == 0) std::sort(all.begin(), all.end(), [](const FileView& a, const FileView& b) { return a.file->getPrice() < b.file->getPrice(); }); // Сортировка по возрастанию цены
            else if (s == 1) std::sort(all.begin(), all.end(), [](const FileView& a, const FileView& b) { return a.file->getPrice() > b.file->getPrice(); }); // Сортировка по убыванию цены
            else if (s == 2) std::sort(all.begin(), all.end(), [](const FileView& a, const FileView& b) { return a.file->getTitle() < b.file->getTitle(); }); // Сортировка по названию
            Table::printFilesTable(all); getInput(); break; // Вывод отсортированной таблицы
        }
        case 4: { // Покупка файла
            auto all = collectAllFiles(shop); // Получение всех файлов
            if (all.empty()) { std::cout << "Нет доступных файлов.\n"; getInput(); break; } // Проверка наличия файлов
            int sel = chooseFileInteractive(all, "Выберите файл для покупки"); // Интерактивный выбор файла
            if (sel < 0) break; // Отмена выбора
            FileView fv = all[sel]; // Получение выбранного файла
            auto f = fv.file; // Указатель на файл
            std::cout << "Подтвердите покупку: " << f->getTitle() << " | Цена: " << f->getPrice() << "\n"; // Подтверждение покупки
            int c = MenuStrings({ "Купить", "Отмена" }, "Подтвердите"); // Меню подтверждения
            if (c != 0) { std::cout << "Покупка отменена\n"; getInput(); break; } // Отмена покупки
            if (!user.buy(f)) { getInput(); break; } // Попытка покупки, проверка баланса
            if (fv.seller) fv.seller->addBalance(f->getPrice()); // Зачисление средств продавцу
            std::cout << "Покупка успешна\n"; user.printReceipt(); // Вывод чека
            int sv = MenuStrings({ "Сохранить чек", "Не сохранять" }, "Чек"); // Меню сохранения чека
            if (sv == 0) { if (user.saveReceipt()) std::cout << "Чек сохранён\n"; else std::cout << "Ошибка сохранения\n"; } // Сохранение чека
            getInput();
            break;
        }
        case 5:
            user.showPurchases(); // Показать историю покупок
            getInput();
            break;
        case 6:
            user.printReceipt(); // Печать чека
            if (MenuStrings({ "Сохранить чек", "Не сохранять" }, "Чек") == 0) { // Меню сохранения чека
                if (user.saveReceipt()) std::cout << "Чек сохранён\n"; else std::cout << "Ошибка\n"; // Сохранение чека
            }
            getInput();
            break;
        case 7:
            std::cout << "Баланс: " << user.getBalance() << "\n"; // Вывод текущего баланса
            getInput();
            break;
        case 8: { // Пополнение баланса
            double amount = readDouble("Введите сумму для пополнения: ", -1.0); // Запрос суммы
            if (amount <= 0.0) { std::cout << "Неверная сумма\n"; getInput(); break; } // Проверка корректности суммы
            user.addBalance(amount); // Пополнение баланса
            std::cout << "Баланс: " << user.getBalance() << "\n"; // Вывод обновленного баланса
            getInput();
            break;
        }
        default:
            break;
        } // switch
    } // while
}

// Меню продавца — теперь без явного пункта "Отправить на модерацию".
// Файлы при добавлении автоматически отправляются в очередь модерации (pending) и
// НЕ остаются в списке продавца в магазине (submitFileForApproval снимает файл из seller).
void sellerMenu(Seller& user, MediaShop& shop) {
    while (true) {
        int choice = MenuStrings({
            "Показать мои файлы",
            "Добавить файл",
            "Редактировать файл",
            "Удалить файл",
            "Баланс",
            "Снять средства",
            "Назад"
            }, std::string("Меню продавца: ") + user.getLogin());

        if (choice < 0 || choice == 6) break;

        switch (choice) {
        case 0:
            user.showUserFiles();
            getInput();
            break;
        case 1: {
            int t = MenuStrings({ "Audio", "Video", "Image", "Отмена" }, "Тип файла");
            if (t < 0 || t == 3) break;
            std::string title = readLine("Название: ");
            if (title.empty()) { std::cout << "Название обязательно\n"; getInput(); break; }
            std::string author = readLine("Автор: "); if (author.empty()) author = "unknown";
            double price = readDouble("Цена: ", 0.0);
            int size = readInt("Размер (кБ): ", 0);
            std::string format = readLine("Формат (mp3/jpg): "); if (format.empty()) format = "unknown";

            // Добавляем файл локально, затем сразу отправляем на модерацию через существующий метод.
            if (t == 0) {
                int dur = readInt("Длительность (с): ", 0);
                std::string album = readLine("Альбом: ");
                user.addFile(std::make_shared<AudioFile>(title, author, price, size, format, dur, album));
            }
            else if (t == 1) {
                int dur = readInt("Длительность (с): ", 0);
                std::string res = readLine("Разрешение: ");
                user.addFile(std::make_shared<VideoFile>(title, author, price, size, format, dur, res));
            }
            else {
                std::string res = readLine("Разрешение: ");
                user.addFile(std::make_shared<ImageFile>(title, author, price, size, format, res));
            }

            // автоматически отправляем послед добавленный файл на модерацию и удаляем из userFiles
            int lastIdx = static_cast<int>(user.getFilesCount());
            if (shop.submitFileForApproval(user.getLogin(), lastIdx)) {
                std::cout << "Файл добавлен и отправлен на модерацию\n";
            }
            else {
                // если что-то пошло не так — файл останется у продавца, уведомим об ошибке
                std::cout << "Файл добавлен, но при отправке на модерацию произошла ошибка\n";
            }
            getInput();
            break;
        }
        case 2: {
            user.showUserFiles();
            int idx = readInt("Номер для редактирования: ", -1);
            auto f = user.getFile(idx);
            if (!f) { std::cout << "Не найден\n"; getInput(); break; }
            f->edit();
            std::cout << "Изменено\n"; getInput();
            break;
        }
        case 3: {
            user.showUserFiles();
            int idx = readInt("Номер для удаления: ", -1);
            if (user.removeFile(idx)) std::cout << "Удалено\n"; else std::cout << "Ошибка\n";
            getInput();
            break;
        }
        case 4:
            std::cout << "Баланс: " << user.getBalance() << "\n"; getInput();
            break;
        case 5: {
            double amount = readDouble("Снять сумму: ", -1.0);
            if (amount <= 0.0) { std::cout << "Неверная сумма\n"; getInput(); break; }
            else if (amount > user.getBalance()) { std::cout << "Недостаточно средств\n"; getInput(); break; }
            user.spend(amount);
            std::cout << "Снято. Остаток: " << user.getBalance() << "\n"; getInput();
            break;
        }
        default:
            break;
        } // switch
    } // while
}

// Меню администратора — switch
void adminMenu(Admin& admin, MediaShop& shop) {
    while (true) {
        int choice = MenuStrings({
            "Список продавцов",
            "Показать файлы продавца",
            "Удалить файл продавца",
            "Очередь модерации",
            "Одобрить следующий",
            "Отклонить следующий",
            "Назад"
            }, std::string("Меню администратора: ") + admin.getLogin());

        if (choice < 0 || choice == 6) break;

        switch (choice) {
        case 0:
            shop.showSellers();
            getInput();
            break;
        case 1: {
            shop.showSellers();
            int sidx = readInt("Номер продавца: ", -1);
            if (sidx < 1 || sidx > static_cast<int>(shop.getSellers().size())) { std::cout << "Неверный индекс\n"; getInput(); break; }
            shop.getSellers()[sidx - 1]->showUserFiles(); getInput();
            break;
        }
        case 2: {
            shop.showSellers();
            int sidx = readInt("Номер продавца: ", -1);
            if (sidx < 1 || sidx > static_cast<int>(shop.getSellers().size())) { std::cout << "Неверный индекс\n"; getInput(); break; }
            auto s = shop.getSellers()[sidx - 1];
            s->showUserFiles();
            int fidx = readInt("Номер файла: ", -1);
            if (s->removeFile(fidx)) std::cout << "Удалено\n"; else std::cout << "Ошибка\n";
            getInput();
            break;
        }
        case 3:
            shop.listPending();
            getInput();
            break;
        case 4:
            if (shop.approveNext()) std::cout << "Одобрено\n"; else std::cout << "Очередь пуста\n";
            getInput();
            break;
        case 5:
            if (shop.rejectNext()) std::cout << "Отклонено\n"; else std::cout << "Очередь пуста\n";
            getInput();
            break;
        default:
            break;
        } // switch
    } // while
}