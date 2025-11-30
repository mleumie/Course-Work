#include <iostream>
#include "Auth.h"
#include "User.h"
#include "MediaFile.h"
#include "MediaShop.h"
#include <memory>
#include <windows.h>

void buyerMenu(Buyer& user, MediaShop& shop);
void sellerMenu(Seller& user, MediaShop& shop);
void adminMenu(Admin& user, MediaShop& shop);

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");
    MediaShop shop;
    shop.loadFromFile("shop.txt");

    std::string s = "[Аудио]";

    Auth auth;

    while (true) {
        AuthUser* authUser = nullptr;
        while (!authUser) {
            std::cout << "1. Вход\n2. Регистрация \n3. Регистрация (продавец)\n4. Регистрация (админ)\n0. Выход\n> ";
            int choice;
            std::cin >> choice;
            if (choice == 1) authUser = auth.loginUser();
            else if (choice == 2) auth.registerUser(AuthRole::buyer);
            else if (choice == 3) auth.registerUser(AuthRole::seller);
            else if (choice == 4) auth.registerUser(AuthRole::admin);
            else if (choice == 0) {
                shop.saveToFile("shop.txt");
                return 0;
            }
        }

        std::cout << "Вход выполнен: " << authUser->login << " (" << ((authUser->role == AuthRole::buyer) ? "buyer" : (authUser->role == AuthRole::seller ? "seller" : "admin")) << ")\n";

        if (authUser->role == AuthRole::buyer) {
            Buyer buyer(authUser->login, 130.0);
            buyerMenu(buyer, shop);
        }
        else if (authUser->role == AuthRole::seller) {
  
            std::shared_ptr<Seller> s = shop.findSeller(authUser->login);
            if (!s) {
                
                s = std::make_shared<Seller>(authUser->login, 0.0);
                shop.addSeller(s);
                std::cout << "Создан новый аккаунт продавца в магазине для логина: " << authUser->login << "\n";
            }
            sellerMenu(*s, shop);
        }
        else if (authUser->role == AuthRole::admin) {
            Admin admin(authUser->login, 0.0);
            adminMenu(admin, shop);
        }
    }
    return 0;
}