#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
#include <limits>

#include "Validation.h"
#include "Client.h"
#include "Encryption.h"
#include "Admin.h"
#include "SuperAdmin.h"
#include "Menu.h"

void Validation::registration(std::string& login, std::string& passwd) {
    char ch{};
    bool check = false;

    std::cout << "¬ведите логин: " << std::endl;

    ch = getchar();

    do {
        login += ch;

        ch = getchar();
    } while (ch != '\n' && ch != '\r');

    std::cout << "¬ведите пароль:" << std::endl;

    do {

        ch = _getch();

        if (ch == '\b' && !passwd.empty())
        {
            passwd.erase(passwd.size() - 1);
            std::cout << "\b \b";
        }
        else if (ch != '\n' && ch != '\r') {
            std::cout << '*';
            passwd += ch;
        }
    } while (ch != '\n' && ch != '\r');

    std::cout << std::endl;
}

int Validation::authUser(std::vector<Client> users, std::string login, std::string passwd, role role) {
    int i = 0;

    for (i; i < users.size(); i++) {
        std::string temp = users.at(i).getUser().getSalt() + passwd;
        toHash(temp);

        if (users.at(i).getUser().getHash() == temp && users.at(i).getUser().getLogin() == login
            && users.at(i).getUser().getRole() == static_cast<int>(role)) {
            return i;
        }
    }

    return i;
}

int Validation::authAdmin(std::vector<Admin> users, std::string login, std::string passwd, role role) {
    int i = 0;

    for (i; i < users.size(); i++) {
        std::string temp = users.at(i).getSalt() + passwd;
        toHash(temp);

        if (users.at(i).getHash() == temp && users.at(i).getLogin() == login && users.at(i).getRole() == static_cast<int>(role)) {
            return i;
        }
    }

    return i;
}

int Validation::authSuperAdmin(SuperAdmin& user, std::string login, std::string passwd, role role) {
    std::string temp = user.getSalt() + passwd;
    toHash(temp);

    if (user.getHash() == temp && user.getLogin() == login && user.getRole() == static_cast<int>(role)) {
        return 1;
    }

    return 0;
}