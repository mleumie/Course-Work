#include <iostream>
#include "Auth.h"
#include "User.h"
#include "MediaFile.h"
#include "MediaShop.h"
#include <memory>
#include <clocale>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

void buyerMenu(Buyer& user, MediaShop& shop);
void sellerMenu(Seller& user, MediaShop& shop);
void adminMenu(Admin& admin, MediaShop& shop);

int main() {
    try {
#ifdef _WIN32
        SetConsoleOutputCP(1251);
        SetConsoleCP(1251);
#endif
        setlocale(LC_ALL, "Russian");
        
        MediaShop shop;
        shop.loadFromFile("shop.txt");

        // Create default sellers if not loaded from file
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
                std::cout << "\n=== MediaShop - Sistema kupli-prodazhi mediafajlov ===\n";
                std::cout << "1. Vhod\n";
                std::cout << "2. Registracija (pokupatel)\n";
                std::cout << "3. Registracija (prodavec)\n";
                std::cout << "4. Registracija (admin)\n";
                std::cout << "0. Vyhod\n> ";
                
                int choice;
                if (!(std::cin >> choice)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Nevernyj vvod\n";
                    continue;
                }
                
                if (choice == 1) authUser = auth.loginUser();
                else if (choice == 2) auth.registerUser(AuthRole::buyer);
                else if (choice == 3) auth.registerUser(AuthRole::seller);
                else if (choice == 4) auth.registerUser(AuthRole::admin);
                else if (choice == 0) {
                    shop.saveToFile("shop.txt");
                    std::cout << "Dannye sohraneny. Do svidanija!\n";
                    return 0;
                }
            }

            std::cout << "Vhod vypolnen: " << authUser->login << " (";
            if (authUser->role == AuthRole::buyer) std::cout << "pokupatel";
            else if (authUser->role == AuthRole::seller) std::cout << "prodavec";
            else std::cout << "admin";
            std::cout << ")\n";

            if (authUser->role == AuthRole::buyer) {
                Buyer buyer(authUser->login, 130.0);
                buyerMenu(buyer, shop);
            }
            else if (authUser->role == AuthRole::seller) {
                std::shared_ptr<Seller> s = shop.findSeller(authUser->login);
                if (!s) {
                    s = std::make_shared<Seller>(authUser->login, 0.0);
                    shop.addSeller(s);
                    std::cout << "Sozdan novyj profil prodavca: " << authUser->login << "\n";
                }
                sellerMenu(*s, shop);
            }
            else if (authUser->role == AuthRole::admin) {
                Admin admin(authUser->login, 0.0);
                adminMenu(admin, shop);
            }
            
            // Save after each session
            shop.saveToFile("shop.txt");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Kriticheskaja oshibka: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}