#include <iostream>
#include "Auth.h"
#include "User.h"
#include "MediaFile.h"
#include "MediaShop.h"

void buyerMenu(Buyer& user, MediaShop& shop);
void sellerMenu(Seller& user, MediaShop& shop);
void adminMenu(Admin& user, MediaShop& shop);

int main() {
    setlocale(LC_ALL, "Russian");
    MediaShop shop;
    Seller* seller1 = new Seller("oleg", "1234", 0.0);
    Seller* seller2 = new Seller("masha", "5678", 0.0);

    seller1->addFile(new AudioFile("Song", "Artist", 50, 5, "mp3", 200, "Album"));
    seller1->addFile(new VideoFile("Clip", "Director", 100, 50, "mp4", 1200, "1920x1080"));
    seller2->addFile(new ImageFile("Pic", "Photographer", 30, 2, "jpg", "800x600"));

    shop.addSeller(seller1);
    shop.addSeller(seller2);

    Auth auth;

    while (true) {
        AuthUser* authUser = nullptr;
        while (!authUser) {
            std::cout << "1. Вход\n2. Регистрация покупателя\n3. Регистрация продавца\n4. Регистрация админа\n0. Выход\n> ";
            int choice;
            std::cin >> choice;
            if (choice == 1) authUser = auth.loginUser();
            else if (choice == 2) auth.registerUser(AuthRole::buyer);
            else if (choice == 3) auth.registerUser(AuthRole::seller);
            else if (choice == 4) auth.registerUser(AuthRole::admin);
            else if (choice == 0) {
                delete seller1;
                delete seller2;
                return 0;
            }
        }

        if (authUser->role == AuthRole::buyer) {
            Buyer buyer(authUser->login, "", 130.0);
            buyerMenu(buyer, shop);
        }
        else if (authUser->role == AuthRole::seller) {
            if (authUser->login == "oleg") sellerMenu(*seller1, shop);
            else if (authUser->login == "masha") sellerMenu(*seller2, shop);
            else {
                Seller seller(authUser->login, "", 0.0);
                sellerMenu(seller, shop);
            }
        }
        else if (authUser->role == AuthRole::admin) {
            Admin admin(authUser->login, "", 0.0);
            adminMenu(admin, shop);
        }
    }
    return 0;
}