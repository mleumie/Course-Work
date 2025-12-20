#include <iostream>
#include <string>
#include <algorithm>

#include "StringGen.h"
#include "SuperAdmin.h"
#include "Roles.h"
#include "InputCheck.h"
#include "Transactions.h"
#include "Validation.h"

SuperAdmin* SuperAdmin::instance = 0;

SuperAdmin::SuperAdmin() : Admin() {
    this->setRole(role::superadmin);
}

SuperAdmin::SuperAdmin(const SuperAdmin& admin) : Admin() {
    this->setRole(role::superadmin);
}

SuperAdmin& SuperAdmin::operator= (SuperAdmin& admin) {
    if (this == &admin) {
        return *this;
    }

    this->setLogin(admin.getInstance()->getLogin());
    this->setSalt(admin.getInstance()->getSalt());
    this->setHashedPassword(admin.getInstance()->getHash());
    this->setRole(role::superadmin);

    return *this;
}

std::vector<Admin>::iterator SuperAdmin::findAdmin(std::string login, std::vector<Admin>& admins, char ch) {
    auto it = admins.begin();

    for (it; it != admins.end(); it++) {
        if (it->getLogin() == login) {
            return it;
        }
    }

    return it;
}

std::ostream& operator<<(std::ostream& stream, SuperAdmin& admin) {
    if (!stream.bad() && !stream.fail()) {
        stream << admin.getLogin() << "\n";
        stream << admin.getHash() << "\n";
        stream << admin.getSalt() << "\n";
        stream << admin.getRole() << "\n";
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, SuperAdmin& admin) {
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

SuperAdmin::SuperAdmin(std::string login, std::string passwd, std::string salt) : Admin(login, passwd, salt) {
    this->setRole(role::superadmin);
}

Admin SuperAdmin::findAdmin(std::string login, std::vector<Admin>& admins) {
    auto it = admins.begin();
    Admin temp;

    for (it; it != admins.end(); it++) {
        if (it->getLogin() == login) {
            return *it;
        }
    }

    return temp;
}

void SuperAdmin::sortAdmins(std::vector<Admin>& admins) {
    std::sort(admins.begin(), admins.end());
}

void SuperAdmin::changeAdmin(std::string login, std::vector<Admin>& admins) {
    char ch = 60;
    auto it = findAdmin(login, admins, ch);
    std::string str_temp, login_temp;

    if (it != admins.end()) {
        Admin temp = *it;
        bool active = true;

        while (active) {
            int counter;

            std::cout << "Выберите пункт меню\n";
            std::cout << "1.Изменить логин админу\n";
            std::cout << "2.Изменить пароль админу\n";
            std::cout << "3.Выход\n";

            inputCheck(counter);

            switch (counter) {
            default: {
                std::cout << "Некорректный ввод\n";
                system("pause");
                break;
            }
            case 1: {
                std::string cont;
                std::string new_login;
                Admin temp_adm = temp, prev = *it;

                new_login = temp_adm.changeLogin();
                Transaction<Admin> trans(&prev);

                temp_adm.setLogin(new_login);

                if (!trans.startTransaction(&temp_adm)) {
                    std::cout << "Что-то пошло не так!\n";
                }

                std::cout << "Вы точно хотите продолжить? Yes/No\n";
                getline(std::cin, cont);

                if (cont == "Yes" || cont == "yes" || cont == "YES") {
                    std::cout << "Операция продолжена!\n";
                    temp = trans.commitTransaction();
                }
                else {
                    std::cout << "Операция отменена!\n";
                    trans.denyTransaction();
                }

                break;
            }
            case 2: {
                std::string cont;
                std::string new_password;
                Admin temp_adm = temp, prev = *it;

                new_password = temp_adm.changePassword();
                Transaction<Admin> trans(&prev);

                temp_adm.setHash(new_password);

                if (!trans.startTransaction(&temp_adm)) {
                    std::cout << "Что-то пошло не так!\n";
                }

                std::cout << "Вы точно хотите продолжить? Yes/No\n";
                getline(std::cin, cont);

                if (cont == "Yes" || cont == "yes" || cont == "YES") {
                    std::cout << "Операция продолжена!\n";
                    temp = trans.commitTransaction();
                }
                else {
                    std::cout << "Операция отменена!\n";
                    trans.denyTransaction();
                }

                break;
            }
            case 3: {
                std::cout << "Осуществляется выход!\n";
                *it = temp;

                system("pause");
                active = false;
            }
                  system("cls");
            }
        }
    }
    else {
        std::cout << "Админ не был найден!\n";
        system("pause");
    }
}

void SuperAdmin::deleteAdmin(std::string login, std::vector<Admin>& admins) {
    char ch = 60;
    auto it = findAdmin(login, admins, ch);

    if (it != admins.end()) {
        admins.erase(it);
    }
    else {
        std::cout << "Нет такого пользователя!\n";
    }
}

void SuperAdmin::addAdmin(std::vector<Admin>& admins) {
    auto it = admins.begin();
    std::string temp, login, password;

    Validation::registration(login, password);

    for (it; it != admins.end(); it++) {
        if (it->getLogin() == login) {
            break;
        }
    }

    if (it == admins.end()) {
        Admin temp_usr(login, password, StringGen::generate());
        admins.push_back(temp_usr);

        std::cout << "Регистрация прошла успешно!\n";
        system("pause");
    }
    else {
        std::cout << "Регистрация не завершена! Такой логин уже есть!\n";
        system("pause");
    }
}