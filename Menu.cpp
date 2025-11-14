//Для хранения коллекций пользователей и медиафайлов применяются векторы(std::vector), обеспечивающие быстрый последовательный доступ.
//Поиск элементов по идентификаторам и другим ключевым полям организован через ассоциативные контейнеры(std::map),
//

#include "User.h"
#include "MediaFile.h"
#include "MediaShop.h"
#include <iostream>

void buyerMenu(Buyer& user, MediaShop& shop) {
    int sellerChoice = -1;
    while (sellerChoice != 0) {
        std::cout << "\nВыберите продавца для просмотра товаров:\n";
        shop.showSellers();
        std::cout << "0. Назад\n> ";
        std::cin >> sellerChoice;
        if (sellerChoice == 0) break;
        Seller* chosenSeller = shop.getSeller(sellerChoice);
        if (!chosenSeller) {
            std::cout << "Нет такого продавца!\n";
            continue;
        }
        int choice = -1;
        while (choice != 0) {
            std::cout << "\n1. Показать каталог продавца\n2. Купить файл\n3. Профиль\n4. Покупки\n5. Пополнить баланс\n0. Назад\n> ";
            std::cin >> choice;
            switch (choice) {
            case 1:
                chosenSeller->showUserFiles();
                break;
            case 2: {
                chosenSeller->showUserFiles();
                std::cout << "Введите номер файла для покупки: ";
                int i; std::cin >> i;
                user.buy(chosenSeller->getFile(i));
                break;
            }
            case 3:
                user.showInfo();
                break;
            case 4:
                user.showPurchases();
                break;
            case 5: {
                std::cout << "Введите сумму для пополнения: ";
                double sum; std::cin >> sum;
                user.addBalance(sum);
                std::cout << "Баланс пополнен.\n";
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Неизвестная команда!\n";
                break;
            }
        }
    }
}

void sellerMenu(Seller& user, MediaShop& shop) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n1. Добавить аудио\n2. Добавить видео\n3. Добавить изображение\n4. Файлы\n5. Профиль\n0. Выход\n> ";
        std::cin >> choice;
        switch (choice) {
        case 1: {
            std::string t, a, f, al;
            int s, d;
            double p;
            std::cout << "Название: "; std::cin >> t;
            std::cout << "Автор: "; std::cin >> a;
            std::cout << "Формат: "; std::cin >> f;
            std::cout << "Цена: "; std::cin >> p;
            std::cout << "Размер (МБ): "; std::cin >> s;
            std::cout << "Длительность (сек): "; std::cin >> d;
            std::cout << "Альбом: "; std::cin >> al;
            user.addFile(new AudioFile(t, a, p, s, f, d, al));
            break;
        }
        case 2: {
            std::string t, a, f, r;
            int s, d;
            double p;
            std::cout << "Название: "; std::cin >> t;
            std::cout << "Автор: "; std::cin >> a;
            std::cout << "Формат: "; std::cin >> f;
            std::cout << "Цена: "; std::cin >> p;
            std::cout << "Размер (МБ): "; std::cin >> s;
            std::cout << "Длительность (сек): "; std::cin >> d;
            std::cout << "Разрешение: "; std::cin >> r;
            user.addFile(new VideoFile(t, a, p, s, f, d, r));
            break;
        }
        case 3: {
            std::string t, a, f, r;
            int s;
            double p;
            std::cout << "Название: "; std::cin >> t;
            std::cout << "Автор: "; std::cin >> a;
            std::cout << "Формат: "; std::cin >> f;
            std::cout << "Цена: "; std::cin >> p;
            std::cout << "Размер (МБ): "; std::cin >> s;
            std::cout << "Разрешение: "; std::cin >> r;
            user.addFile(new ImageFile(t, a, p, s, f, r));
            break;
        }
        case 4:
            user.showUserFiles();
            break;
        case 5:
            user.showInfo();
            break;
        case 0:
            break;
        default:
            std::cout << "Неизвестная команда!\n";
            break;
        }
    }
}

void adminMenu(Admin& user, MediaShop& shop) {
    int sellerChoice = -1;
    while (sellerChoice != 0) {
        std::cout << "\nВыберите продавца для просмотра его товаров/удаления:\n";
        shop.showSellers();
        std::cout << "0. Назад\n> ";
        std::cin >> sellerChoice;
        if (sellerChoice == 0) break;
        Seller* chosenSeller = shop.getSeller(sellerChoice);
        if (!chosenSeller) {
            std::cout << "Нет такого продавца!\n";
            continue;
        }
        int choice = -1;
        while (choice != 0) {
            std::cout << "\n1. Показать каталог продавца\n2. Удалить файл\n3. Профиль\n0. Назад\n> ";
            std::cin >> choice;
            switch (choice) {
            case 1:
                chosenSeller->showUserFiles();
                break;
            case 2: {
                chosenSeller->showUserFiles();
                std::cout << "Введите номер файла для удаления: ";
                int i;
                std::cin >> i;
                user.removeFile(chosenSeller, i);
                break;
            }
            case 3:
                user.showInfo();
                break;
            case 0:
                break;
            default:
                std::cout << "Неверный ввод\n";
                break;
            }
        }
    }
}