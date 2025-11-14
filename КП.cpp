////21. Разработка автоматизированной системы для организации работы
////по купле - продаже медиафайлов
//
////проверки на ввод
// 

// Удаление файлов через admin и seller есть
//Редактирование(например, изменение цены файла) — нет


//Нет поиска, сортировки, фильтрации по каталогу файлов или пользователям.

//Покупатель может купить файл, покупки можно посмотреть.
/*❗ Но "отчёт" как отдельная печатная форма не реализован*/

//Дружественных функций нет.
//
//Пространства имён
// Нет пользовательских пространств имён(namespace).
//
//Обработка ошибок средствами C++
//Ввод и чтение файлов проверяется, но try - catch нет(можно добавить для примера).
//
//Перегрузка методов и операторов
//❌ Перегрузки операторов(например, << , >> ) — нет.
//
//Шаблоны классов / методов
//❌ Нет шаблонов(template).
//
//Статические методы и поля
//❌ Нет.
//
//Динамическое выделение памяти, smart pointers
//❗ Используется new / delete, но умных указателей(smart pointers) нет.
//
//Потоки C++, перегрузка операторов ввода / вывода, контроль ошибок
//✔️ Потоки используются,
//❌ перегрузка операторов ввода / вывода для пользовательских классов — нет.
//


//#include "Auth.h"
//#include <iostream>
//#include <string>
//#include <vector>
//#include <windows.h>
//
//using namespace std;
//
//class Seller;
//class MediaShop;
//class Buyer;
//class Admin;
//class User;
//class MediaFile;
//
//enum Role { buyer, seller, admin };
//
//class MediaFile {
//protected:
//    std::string title;
//    std::string author;
//    std::string format;
//    double price;
//    int size;
//public:
//    MediaFile(std::string t, std::string a, double p, int s, std::string f)
//        : title(t), author(a), price(p), size(s), format(f) {
//    }
//    virtual void printInfo() const = 0;
//    virtual ~MediaFile() {}
//    double getPrice() const { return price; }
//    std::string getTitle() const { return title; }
//    std::string getAuthor() const { return author; }
//    std::string getFormat() const { return format; }
//    int getSize() const { return size; }
//};
//
//class AudioFile : public MediaFile {
//protected:
//    int duration;
//    std::string album;
//public:
//    AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al)
//        : MediaFile(t, a, p, s, f), duration(d), album(al) {
//    }
//    void printInfo() const override {
//        std::cout << "[Аудио] " << title << ", Автор: " << author << ", Формат: " << format
//            << ", Цена: " << price << ", Размер: " << size << " МБ, "
//            << "Длительность: " << duration << " сек., Альбом: " << album << std::endl;
//    }
//};
//
//class VideoFile : public MediaFile {
//protected:
//    int duration;
//    std::string resolution;
//public:
//    VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r)
//        : MediaFile(t, a, p, s, f), duration(d), resolution(r) {
//    }
//    void printInfo() const override {
//        std::cout << "[Видео] " << title << ", Автор: " << author << ", Формат: " << format
//            << ", Цена: " << price << ", Размер: " << size << " МБ, "
//            << "Длительность: " << duration << " сек., Разрешение: " << resolution << std::endl;
//    }
//};
//
//class ImageFile : public MediaFile {
//protected:
//    std::string resolution;
//public:
//    ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r)
//        : MediaFile(t, a, p, s, f), resolution(r) {
//    }
//    void printInfo() const override {
//        std::cout << "[Изображение] " << title << ", Автор: " << author << ", Формат: " << format
//            << ", Цена: " << price << ", Размер: " << size << " МБ, "
//            << "Разрешение: " << resolution << std::endl;
//    }
//};
//
//class User {
//protected:
//    std::string login;
//    std::string password;
//    Role role;
//    double balance;
//public:
//    User(std::string l, std::string p, double b)
//        : login(l), password(p), balance(b), role(Role::buyer) {
//    }
//    virtual ~User() {}
//    Role getRole() const { return role; }
//    std::string getLogin() const { return login; }
//    double getBalance() const { return balance; }
//    void addBalance(double sum) { balance += sum; }
//    bool spend(double sum) {
//        if (balance >= sum) {
//            balance -= sum;
//            return true;
//        }
//        return false;
//    }
//    void showInfo() {
//        std::cout << "Логин: " << login << ", Роль: ";
//        if (role == Role::buyer) std::cout << "Покупатель";
//        else if (role == Role::seller) std::cout << "Продавец";
//        else std::cout << "Админ";
//        std::cout << ", Баланс: " << balance << std::endl;
//    }
//};
//
//class Buyer : public User {
//    std::vector<MediaFile*> purchases;
//public:
//    Buyer(std::string l, std::string p, double b = 0)
//        : User(l, p, b) {
//        role = Role::buyer;
//    }
//    void buy(MediaFile* item) {
//        if (!item) return;
//        if (spend(item->getPrice())) {
//            purchases.push_back(item);
//            std::cout << "Покупка успешна: " << item->getTitle() << std::endl;
//        }
//        else {
//            std::cout << "Недостаточно средств для покупки: " << item->getTitle() << std::endl;
//        }
//    }
//    void showPurchases() const {
//        std::cout << "Ваши покупки:" << std::endl;
//        if (purchases.empty()) std::cout << "Нет покупок." << std::endl;
//        for (auto item : purchases) item->printInfo();
//    }
//};
//
//class Seller : public User {
//    std::vector<MediaFile*> UserFiles;
//public:
//    Seller(std::string l, std::string p, double b = 0) : User(l, p, b) {
//        role = Role::seller;
//    }
//    void addFile(MediaFile* item) {
//        UserFiles.push_back(item);
//        std::cout << "Файл добавлен в магазин: " << item->getTitle() << std::endl;
//    }
//    void showUserFiles() const {
//        std::cout << "Ваши файлы:\n";
//        if (UserFiles.empty()) std::cout << "Нет файлов.\n";
//        for (auto item : UserFiles) item->printInfo();
//    }
//    MediaFile* getFile(int idx) {
//        if (idx < 1 || idx > UserFiles.size()) return nullptr;
//        return UserFiles[idx - 1];
//    }
//    bool removeFile(int idx) {
//        if (idx < 1 || idx > UserFiles.size()) return false;
//        auto it = UserFiles.begin() + (idx - 1);
//        delete* it;
//        UserFiles.erase(it);
//        return true;
//    }
//    // просмотр статистики продаж и т.д. idk
//};
//
//class MediaShop {
//    std::vector<Seller*> sellers;
//public:
//    void addSeller(Seller* s) { sellers.push_back(s); }
//    std::vector<Seller*>& getSellers() { return sellers; }
//    void showSellers() const {
//        std::cout << "Список продавцов:\n";
//        for (int i = 0; i < sellers.size(); ++i)
//            std::cout << i + 1 << ". " << sellers[i]->getLogin() << std::endl;
//    }
//    Seller* getSeller(int idx) {
//        if (idx < 1 || idx > sellers.size()) return nullptr;
//        return sellers[idx - 1];
//    }
//};
//
//class Admin : public User {
//public:
//    Admin(std::string l, std::string p, double b = 0) : User(l, p, b) {
//        role = Role::admin;
//    }
//    void removeFile(Seller* seller, int idx) {
//        if (seller && seller->removeFile(idx)) {
//            std::cout << "Файл удалён администратором\n";
//        }
//        else {
//            std::cout << "Нет такого файла у продавца\n";
//        }
//    }
//    // удаление\блок пользователей, просмотр статистики 
//};
//
//void buyerMenu(Buyer& user, MediaShop& shop) {
//    int sellerChoice = -1;
//    while (sellerChoice != 0) {
//        std::cout << "\nВыберите продавца для просмотра товаров:\n";
//        shop.showSellers();
//        std::cout << "0. Назад\n> ";
//        std::cin >> sellerChoice;
//        if (sellerChoice == 0) break;
//        Seller* chosenSeller = shop.getSeller(sellerChoice);
//        if (!chosenSeller) {
//            std::cout << "Нет такого продавца!\n";
//            continue;
//        }
//        int choice = -1;
//        while (choice != 0) {
//            std::cout << "\n1. Показать каталог продавца\n2. Купить файл\n3. Профиль\n4. Покупки\n5. Пополнить баланс\n0. Назад\n> ";
//            std::cin >> choice;
//            switch (choice) {
//            case 1:
//                chosenSeller->showUserFiles();
//                break;
//            case 2: {
//                chosenSeller->showUserFiles();
//                std::cout << "Введите номер файла для покупки: ";
//                int i; std::cin >> i;
//                user.buy(chosenSeller->getFile(i));
//                break;
//            }
//            case 3:
//                user.showInfo();
//                break;
//            case 4:
//                user.showPurchases();
//                break;
//            case 5: {
//                std::cout << "Введите сумму для пополнения: ";
//                double sum; std::cin >> sum;
//                user.addBalance(sum);
//                std::cout << "Баланс пополнен.\n";
//                break;
//            }
//            case 0:
//                break;
//            default:
//                std::cout << "Неизвестная команда!\n";
//                break;
//            }
//        }
//    }
//}
//
//void sellerMenu(Seller& user, MediaShop& shop) {
//    int choice = -1;
//    while (choice != 0) {
//        std::cout << "\n1. Добавить аудио\n2. Добавить видео\n3. Добавить изображение\n4. Файлы\n5. Профиль\n0. Выход\n> ";
//        std::cin >> choice;
//        switch (choice) {
//        case 1: {
//            std::string t, a, f, al;
//            int s, d;
//            double p;
//            std::cout << "Название: "; std::cin >> t;
//            std::cout << "Автор: "; std::cin >> a;
//            std::cout << "Формат: "; std::cin >> f;
//            std::cout << "Цена: "; std::cin >> p;
//            std::cout << "Размер (МБ): "; std::cin >> s;
//            std::cout << "Длительность (сек): "; std::cin >> d;
//            std::cout << "Альбом: "; std::cin >> al;
//            user.addFile(new AudioFile(t, a, p, s, f, d, al));
//            break;
//        }
//        case 2: {
//            std::string t, a, f, r;
//            int s, d;
//            double p;
//            std::cout << "Название: "; std::cin >> t;
//            std::cout << "Автор: "; std::cin >> a;
//            std::cout << "Формат: "; std::cin >> f;
//            std::cout << "Цена: "; std::cin >> p;
//            std::cout << "Размер (МБ): "; std::cin >> s;
//            std::cout << "Длительность (сек): "; std::cin >> d;
//            std::cout << "Разрешение: "; std::cin >> r;
//            user.addFile(new VideoFile(t, a, p, s, f, d, r));
//            break;
//        }
//        case 3: {
//            std::string t, a, f, r;
//            int s;
//            double p;
//            std::cout << "Название: "; std::cin >> t;
//            std::cout << "Автор: "; std::cin >> a;
//            std::cout << "Формат: "; std::cin >> f;
//            std::cout << "Цена: "; std::cin >> p;
//            std::cout << "Размер (МБ): "; std::cin >> s;
//            std::cout << "Разрешение: "; std::cin >> r;
//            user.addFile(new ImageFile(t, a, p, s, f, r));
//            break;
//        }
//        case 4:
//            user.showUserFiles();
//            break;
//        case 5:
//            user.showInfo();
//            break;
//        case 0:
//            break;
//        default:
//            std::cout << "Неизвестная команда!\n";
//            break;
//        }
//    }
//}
//
//void adminMenu(Admin& user, MediaShop& shop) {
//    int sellerChoice = -1;
//    while (sellerChoice != 0) {
//        std::cout << "\nВыберите продавца для просмотра его товаров/удаления:\n";
//        shop.showSellers();
//        std::cout << "0. Назад\n> ";
//        std::cin >> sellerChoice;
//        if (sellerChoice == 0) break;
//        Seller* chosenSeller = shop.getSeller(sellerChoice);
//        if (!chosenSeller) {
//            std::cout << "Нет такого продавца!\n";
//            continue;
//        }
//        int choice = -1;
//        while (choice != 0) {
//            std::cout << "\n1. Показать каталог продавца\n2. Удалить файл\n3. Профиль\n0. Назад\n> ";
//            std::cin >> choice;
//            switch (choice) {
//            case 1:
//                chosenSeller->showUserFiles();
//                break;
//            case 2: {
//                chosenSeller->showUserFiles();
//                std::cout << "Введите номер файла для удаления: ";
//                int i;
//                std::cin >> i;
//                user.removeFile(chosenSeller, i);
//                break;
//            }
//            case 3:
//                user.showInfo();
//                break;
//            case 0:
//                break;
//            default:
//                std::cout << "Неверный ввод\n";
//                break;
//            }
//        }
//    }
//}
//
//void mainMenu() {
//    MediaShop shop;
//
//    // Создаём продавцов
//    Seller* seller1 = new Seller("oleg", "1234", 0.0);
//    Seller* seller2 = new Seller("masha", "5678", 0.0);
//
//    // Добавляем тестовые товары каждому продавцу
//    seller1->addFile(new AudioFile("Song", "Artist", 50, 5, "mp3", 200, "Album"));
//    seller1->addFile(new VideoFile("Clip", "Director", 100, 50, "mp4", 1200, "1920x1080"));
//    seller2->addFile(new ImageFile("Pic", "Photographer", 30, 2, "jpg", "800x600"));
//
//    // Добавляем продавцов в магазин
//    shop.addSeller(seller1);
//    shop.addSeller(seller2);
//
//    Buyer buyer("vasya", "pass", 130.0);
//    Admin admin("root", "admin", 0.0);
//
//    int roleChoice;
//    do {
//        std::cout << "\n--- Вход ---\n1. Покупатель\n2. Продавец (Oleg)\n3. Продавец (Masha)\n4. Админ\n0. Выход\n> ";
//        std::cin >> roleChoice;
//        if (roleChoice == 1) {
//            buyerMenu(buyer, shop);
//        }
//        else if (roleChoice == 2) {
//            sellerMenu(*seller1, shop);
//        }
//        else if (roleChoice == 3) {
//            sellerMenu(*seller2, shop);
//        }
//        else if (roleChoice == 4) {
//            adminMenu(admin, shop);
//        }
//    } while (roleChoice != 0);
//
//    // Освобождаем память
//    delete seller1;
//    delete seller2;
//}
//
//int main() {
//    setlocale(LC_ALL, "Russian");
//    mainMenu();
//    return 0;
//}