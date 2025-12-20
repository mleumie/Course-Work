#include <iostream> // для вывода сообщений и работы с потоками данных
#include "Auth.h" // подключение модуля аутентификации
#include "User.h" // подключение модуля работы с пользователями
#include "Roles.h" // подключение определения пользовательских ролей
#include "MediaFile.h" // подключение логики работы с медийными файлами
#include "MediaShop.h" // подключение логики магазина
#include "ConsoleUI.h" // подключение интерфейса с меню и стрелками
#include <memory> // подключение для управления указателями `shared_ptr`
#include <windows.h> // подключение функций работы с консолью Windows
#include <stdexcept> // подключение для обработки исключений std::exception

void buyerMenu(Buyer& user, MediaShop& shop); // меню покупателя
void sellerMenu(Seller& user, MediaShop& shop); // меню продавца
void adminMenu(Admin& user, MediaShop& shop); // меню администратора

int main() {
    SetConsoleOutputCP(1251); 
    SetConsoleCP(1251); 
    setlocale(LC_ALL, "Russian"); 
    MediaShop shop; // Создаётся объект магазина для управления товарами и пользователями
    try { // Попытка загрузить файлы магазина
        shop.load("shop.dat"); } // Загружает данные магазина из файла
    catch (const std::exception& ex) { std::cerr << "Ошибка загрузки shop.dat: " << ex.what() << "\n"; } 
    try { // Попытка загрузить файл очереди ожидания
        shop.loadPending("pending.txt"); // Загружает очередь ожидания из текстового файла
    }
    catch (const std::exception& ex) {
        std::cerr << "Ошибка загрузки pending.txt: " << ex.what() << "\n";
    } 
    Auth auth; // Создание объекта менеджера пользователей для входа/регистрации
    while (true) {
        AuthUser* authUser = nullptr; // Указатель на авторизованного пользователя
        std::vector<std::string> mainMenu = { // Главное меню
            "Вход",
            "Регистрация (покупатель)", 
            "Регистрация (продавец)", 
            "Регистрация (админ)", 
            "Выход" 
        };
        while (!authUser) { // Если пользователь не вошёл в систему, отображается главное меню
            int sel = MenuStrings(mainMenu, "Главное меню"); // Отображает меню, возвращает выбранный пункт
            if (sel < 0 || sel == 4) { // Если выбраны Esc или опция выход
                try { // Попытка сохранить данные магазина перед выходом
                    shop.save("shop.dat"); // Сохраняет товары и состояние магазина
                    shop.savePending("pending.txt"); // Сохраняет очередь ожидания
                }
                catch (const std::exception& ex) {
                    std::cerr << "Ошибка сохранения: " << ex.what() << "\n";
                } // Сообщает об ошибках сохранения
                return 0; // Завершает выполнение программы
            }
            if (sel == 0) { // Пункт входа в систему
                authUser = auth.loginUser(); // Вызов функции логина, возвращает указатель на авторизованного пользователя
                if (!authUser) {
                    getInput();
				} // Если вход неудачен, ждёт нажатия клавиши
            }
            else if (sel == 1) {
                auth.registerUser(Role::Buyer);
                getInput();
            } // Регистрация покупателя 
            else if (sel == 2) {
                auth.registerUser(Role::Seller);
                getInput();
            } // Регистрация продавца
            else if (sel == 3) {
                auth.registerUser(Role::Admin);
                getInput();
            } // Регистрация администратора
        }
        std::cout << "Вы вошли: " << authUser->login << " (" // Вывод информации о пользователе
            << ((authUser->role == Role::Buyer) ? "buyer"
                : (authUser->role == Role::Seller ? "seller" : "admin"))
            << ")\n"; // Показывает логин и роль пользователя
        if (authUser->role == Role::Buyer) {
            Buyer buyer(authUser->login, 0);
            buyerMenu(buyer, shop);
        } // Меню покупателя с объектом Buyer
        else if (authUser->role == Role::Seller) {
            std::shared_ptr<Seller> s = shop.findSeller(authUser->login);
            if (!s) {
                s = std::make_shared<Seller>(authUser->login, 0.0);
                shop.addSeller(s);
                std::cout << "Создан аккаунт продавца: " << authUser->login << "\n";
            }
            sellerMenu(*s, shop);
		} // Меню продавца с объектом Seller
        else if (authUser->role == Role::Admin) {
            Admin admin(authUser->login, 0.0);
            adminMenu(admin, shop);
        } // Меню администратора с объектом Admin
    }
    return 0;
}