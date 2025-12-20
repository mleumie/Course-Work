#include <iostream>
#include <algorithm>
#include <vector>

#include "Admin.h"
#include "StringGen.h"
#include "InputCheck.h"
#include "Table.h"
#include "Transactions.h"
#include "Validation.h"

std::vector<Furniture>::iterator Admin::findFurniture(std::string name, std::vector<Furniture>& furnitures, char ch) {
    auto it = furnitures.begin();

    for (it; it != furnitures.end(); it++) {
        if (it->getName() == name) {
            return it;
        }
    }

    return it;
}

std::vector<Client>::iterator Admin::findUser(std::string login, std::vector<Client>& clients, char ch) {
    auto it = clients.begin();

    for (it; it != clients.end(); it++) {
        if (it->getUser().getLogin() == login) {
            return it;
        }
    }

    return it;
}

std::list<Furniture>::iterator Admin::findFurniture(std::string name, std::list<Furniture>& furnitures, char ch) {
    auto it = furnitures.begin();

    for (it; it != furnitures.end(); it++) {
        if (it->getName() == name) {
            return it;
        }
    }

    return it;
}

std::ostream& operator<<(std::ostream& stream, Admin& admin) {
    if (!stream.bad() && !stream.fail()) {
        stream << admin.getLogin() << "\n";
        stream << admin.getHash() << "\n";
        stream << admin.getSalt() << "\n";
        stream << admin.getRole() << "\n";
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Admin& admin) {
    char temp[MAX_STRING_LENGTH];
    int amount = 0;
    role temp_role;

    if (!stream.bad() && !stream.fail()) {
        stream.getline(temp, MAX_STRING_LENGTH);
        admin.setLogin(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        admin.setHashedPassword(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        admin.setSalt(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);

        temp_role = (role)amount;

        admin.setRole(temp_role);
    }

    return stream;
}

bool Admin::operator<(const Admin& adm) {
    return this->getLogin() < adm.getLogin();
}

bool Admin::operator>(const Admin& adm) {
    return this->getLogin() > adm.getLogin();
}

bool Admin::operator==(const Admin& adm) {
    return this->getLogin() == adm.getLogin();
}

Admin::Admin() : User() {
    this->setLogin("Admin");
    this->setRole(role::admin);
}

Admin::Admin(std::string login, std::string passwd, std::string salt) : User(login, passwd, salt) {
    this->setRole(role::admin);
}

Admin::Admin(const Admin& adm) : User(adm) { }

void Admin::addFurniture(std::vector<Furniture>& furnitures) {
    Furniture newFurniture;

    std::cout << "Введите информацию о новой мебели(название, цену, кол-во в наличии, кол-во компонетов, компоненты):\n";
    std::cin >> newFurniture;

    furnitures.push_back(newFurniture);

    std::cout << "Новая мебель успешно добавлена.\n";
}

void Admin::sortUsers(std::vector<Client>& users) {
    std::sort(users.begin(), users.end());
}

Client Admin::findUser(std::string login, std::vector<Client>& clients) {
    auto it = clients.begin();
    Client usr;

    for (it; it != clients.end(); it++) {
        if (it->getUser().getLogin() == login) {
            return *it;
        }
    }

    return usr;
}

void Admin::deleteUser(std::string login, std::vector<Client>& clients) {
    char ch = 60;
    auto it = findUser(login, clients, ch);

    if (it != clients.end()) {
        clients.erase(it);
    }
    else {
        std::cout << "Нет такого пользователя!\n";
    }
}

void Admin::addUser(std::vector<Client>& clients) {
    auto it = clients.begin();
    std::string temp, login, password;

    Validation::registration(login, password);

    for (it; it != clients.end(); it++) {
        if (it->getUser().getLogin() == login) {
            break;
        }
    }

    if (it == clients.end()) {
        std::cout << "Пожалуйста, введите ФИО для аккаунта\n";
        getline(std::cin, temp);

        User temp_usr(login, password, StringGen::generate());
        Client cl(temp, temp_usr);

        clients.push_back(cl);

        std::cout << "Регистрация прошла успешно!\n";
        system("pause");
    }
    else {
        std::cout << "Регистрация не завершена! Такой логин уже есть!\n";
        system("pause");
    }
}

void Admin::changeUser(std::string name, std::vector<Client>& clients, std::vector<Furniture>& furnitures) {
    char ch = 60;
    auto it = findUser(name, clients, ch);
    Client temp = *it;
    std::string str_temp, login_temp;
    int int_temp = 0;

    if (it != clients.end()) {
        bool active = true;

        while (active) {
            int counter;

            std::cout << "Выберите пункт меню\n";
            std::cout << "1.Изменить ФИО\n";
            std::cout << "2.Изменить логин пользователя\n";
            std::cout << "3.Изменить пароль пользователя\n";
            std::cout << "4.Изменить баланс\n";
            std::cout << "5.Добавить мебель\n";
            std::cout << "6.Удалить мебель\n";
            std::cout << "7.Выход\n";

            inputCheck(counter);

            switch (counter) {
            default: {
                std::cout << "Некорректный ввод\n";
                system("pause");
                break;
            }
            case 1: {
                std::cout << "Введите новое название:\n";
                getline(std::cin, str_temp);

                temp.setName(str_temp);

                system("pause");
                break;
            }
            case 2: {
                std::cout << "Введите логин пользователя:\n";
                getline(std::cin, login_temp);

                auto it_user = findUser(login_temp, clients, ch);

                std::string cont;
                std::string new_login;
                User temp = it_user->getUser(), prev = it_user->getUser();

                new_login = temp.changeLogin();
                Transaction<User> trans(&prev);

                temp.setLogin(new_login);

                if (!trans.startTransaction(&temp)) {
                    std::cout << "Что-то пошло не так!\n";
                }

                std::cout << "Вы точно хотите продолжить? Yes/No\n";
                getline(std::cin, cont);

                if (cont == "Yes" || cont == "yes" || cont == "YES") {
                    std::cout << "Операция продолжена!\n";
                    it_user->setUser(trans.commitTransaction());
                }
                else {
                    std::cout << "Операция отменена!\n";
                    trans.denyTransaction();
                }
                break;
            }
            case 3: {
                std::cout << "Введите логин пользователя:\n";
                getline(std::cin, login_temp);

                auto it_user = findUser(login_temp, clients, ch);

                std::string cont;
                std::string new_password;
                User temp = it_user->getUser(), prev = it_user->getUser();

                new_password = temp.changePassword();
                Transaction<User> trans(&prev);

                temp.setHash(new_password);

                if (!trans.startTransaction(&temp)) {
                    std::cout << "Что-то пошло не так!\n";
                }

                std::cout << "Вы точно хотите продолжить? Yes/No\n";
                getline(std::cin, cont);

                if (cont == "Yes" || cont == "yes" || cont == "YES") {
                    std::cout << "Операция продолжена!\n";
                    it_user->setUser(trans.commitTransaction());
                }
                else {
                    std::cout << "Операция отменена!\n";
                    trans.denyTransaction();
                }
                break;
            }
            case 4: {
                std::cout << "Введите новый баланс:\n";
                inputCheck(int_temp);

                if (int_temp > 0) {
                    temp.setMoney(int_temp);
                }
                else {
                    std::cout << "Некорректное значение баланса!\n";
                }

                system("pause");
                break;
            }
            case 5: {
                system("pause");
                std::cout << "Введите название билета:\n";
                getline(std::cin, str_temp);

                std::cout << "Введите логин пользователя:\n";
                getline(std::cin, login_temp);

                auto it_user = findUser(login_temp, clients, ch);

                if (it_user != clients.end()) {
                    auto user_list = it_user->getFurniture();

                    auto it_ticket = findFurniture(str_temp, user_list, ch);

                    if (it_ticket != user_list.end()) {
                        it_user->addFurniture(*it_ticket);
                        it_user->setFurniture(user_list);
                    }
                    else {
                        std::cout << "Нет такой мебели!\n";
                    }
                }
                else {
                    std::cout << "Нет такого пользователя!\n";
                }

                system("pause");
                break;
            }
            case 6: {
                std::cout << "Введите название нужной мебели:\n";
                getline(std::cin, str_temp);

                std::cout << "Введите логин пользователя:\n";
                getline(std::cin, login_temp);

                auto it_user = findUser(login_temp, clients, ch);

                if (it_user != clients.end()) {
                    auto user_list = it_user->getFurniture();

                    auto it_ticket = findFurniture(str_temp, user_list, ch);

                    if (it_ticket != user_list.end()) {
                        user_list.erase(it_ticket);
                        it_user->setFurniture(user_list);
                    }
                    else {
                        std::cout << "Нет такой мебели!\n";
                    }
                }
                else {
                    std::cout << "Нет такого пользователя!\n";
                }
                break;
            }
            case 7: {
                std::cout << "Осуществляется выход!\n";
                *it = temp;

                system("pause");
                active = false;
            }

            }
            system("cls");
        }
    }
    else {
        std::cout << "Пользователь не был найден!\n";
    }
}

void Admin::sortFurniture(std::vector<Furniture>& furnitures) {
    std::sort(furnitures.begin(), furnitures.end());
}

Furniture Admin::findFurniture(std::string name, std::vector<Furniture>& furnitures) {
    auto it = furnitures.begin();
    Furniture temp;

    for (it; it != furnitures.end(); it++) {
        if (it->getName() == name) {
            return *it;
        }
    }

    return temp;
}

void Admin::deleteFurniture(std::string name, std::vector<Furniture>& furnitures) {
    char ch = 60;
    auto it = findFurniture(name, furnitures, ch);

    if (it != furnitures.end()) {
        furnitures.erase(it);
    }
    else {
        std::cout << "Нет такой мебели!\n";
    }
}

void Admin::changeFurniture(std::string name, std::vector<Furniture>& furnitures) {
    char ch = 60;
    auto it = findFurniture(name, furnitures, ch);
    std::string str_temp;
    int int_temp;

    if (it != furnitures.end()) {
        Furniture temp = *it;
        bool active = true;

        while (active) {
            int counter;

            std::cout << "Выберите пункт меню\n";
            std::cout << "1.Изменить название\n";
            std::cout << "2.Изменить цену\n";
            std::cout << "3.Изменить количество\n";
            std::cout << "4.Выход\n";

            inputCheck(counter);

            switch (counter) {
            default: {
                std::cout << "Некорректный ввод\n";
                system("pause");
                break;
            }
            case 1: {
                std::cout << "Введите новое название:\n";
                getline(std::cin, str_temp);

                temp.setName(str_temp);

                system("pause");
                break;
            }
            case 2: {
                std::cout << "Введите новую цену:\n";
                inputCheck(int_temp);

                if (int_temp > 0) {
                    temp.setPrice(int_temp);
                }
                else {
                    std::cout << "Некорректное значение цены!\n";
                }

                system("pause");
                break;
            }
            case 3: {
                std::cout << "Введите новое количество:\n";
                inputCheck(int_temp);

                if (int_temp > 0) {
                    temp.setAmount(int_temp);
                }
                else {
                    std::cout << "Некорректное количество!\n";
                }

                system("pause");
                break;
            }
            case 4: {
                std::cout << "Осуществляется выход!\n";
                *it = temp;

                system("pause");
                active = false;
            }

            }
            system("cls");
        }
    }
    else {
        std::cout << "Нужная мебель не была найдена!\n";
    }
}