#include <iostream>
#include "Auth.h"
#include "User.h"
#include "MediaFile.h"
#include "MediaShop.h"
#include <memory>

void buyerMenu(Buyer& user, MediaShop& shop);
void sellerMenu(Seller& user, MediaShop& shop);
void adminMenu(Admin& user, MediaShop& shop);

int main() {
    setlocale(LC_ALL, "Russian");
    MediaShop shop;
    shop.loadFromFile("shop.txt");

    if (!shop.findSeller("oleg")) {
        auto seller1 = std::make_shared<Seller>("oleg", 0.0);
        seller1->addFile(std::make_shared<AudioFile>("Song", "Artist", 50.0, 5, "mp3", 200, "Album"));
        seller1->addFile(std::make_shared<VideoFile>("Clip", "Director", 100.0, 50, "mp4", 1200, "1920x1080"));
        shop.addSeller(seller1);
    }
    if (!shop.findSeller("masha")) {
        auto seller2 = std::make_shared<Seller>("masha", 0.0);
        seller2->addFile(std::make_shared<ImageFile>("Pic", "Photographer", 30.0, 2, "jpg", "800x600"));
        shop.addSeller(seller2);
    }

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