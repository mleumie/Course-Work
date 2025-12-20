#define ONE 1

#include <iostream>
#include <string>
#include <iomanip>

#include "Client.h"
#include "Furniture.h"
#include "InputCheck.h"

std::ostream& operator<<(std::ostream& stream, Client& usr) {
    if (!stream.bad() && !stream.fail()) {
        stream << usr.name << "\n";
        stream << usr.money << "\n";

        stream << usr.furnitures.size() << "\n";

        for (auto it = usr.furnitures.begin(); it != usr.furnitures.end(); it++) {
            stream << (*it) << "\n";
        }

        stream << usr.user;
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Client& usr) {
    char temp[MAX_STRING_LENGTH];
    int amount = 0;
    role temp_role = role::user;
    Furniture temp_furniture;

    if (!stream.bad() && !stream.fail()) {
        stream.getline(temp, MAX_STRING_LENGTH);
        usr.setName(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);
        usr.setMoney(amount);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);

        while (amount > 0) {
            stream >> temp_furniture;
            usr.furnitures.push_back(temp_furniture);
        }

        stream >> usr.user;
    }

    return stream;
}

bool Client::operator<(const Client& client) {
    return this->name < client.name;
}

bool Client::operator>(const Client& client) {
    return this->name > client.name;
}

bool Client::operator==(const Client& client) {
    return this->name == client.name;
}

std::string Client::getName() {
    return name;
}

void Client::setName(std::string name) {
    this->name = name;
}

std::list<Furniture> Client::getFurniture() {
    return furnitures;
}

void Client::setFurniture(std::list<Furniture> furnitures) {
    this->furnitures = furnitures;
}

int Client::getMoney() {
    return money;
}

void Client::setMoney(int money) {
    if (money > 0) {
        this->money = money;
    }
}

Furniture Client::getFurniture(int number) {
    Furniture temp;
    auto it = furnitures.begin();
    number--;

    if (number < furnitures.size()) {
        std::advance(it, number);
        return *it;
    }
    else {
        std::cout << "Нет такой мебели!";
    }

    return *it;
}

Furniture Client::getFurniture(std::string name) {
    auto it = furnitures.begin();

    for (it; it != furnitures.end(); it++) {
        if (it->getName() == name) {
            return *it;
        }
    }

    return *it;
}

User Client::getUser() {
    return user;
}

void Client::setUser(User usr) {
    user = usr;
}

void Client::addFurniture(Furniture furniture) {
    auto it = std::find(furnitures.begin(), furnitures.end(), furniture);

    if (it == furnitures.end()) {
        furniture.setAmount(ONE);
        furnitures.push_back(furniture);
    }
    else {
        int temp = it->getAmount();

        it->setAmount(++temp);
    }
}

void Client::buy(std::vector<Furniture>& furnitures, std::string furnitures_name) {

    for (auto i = furnitures.begin(); i != furnitures.end(); i++) {
        if (i->getName() == furnitures_name) {
            int amount = i->getAmount();
            if (amount > 0 && this->money >= i->getPrice()) {
                this->money -= i->getPrice();

                i->setAmount(--amount);
                this->addFurniture(*i);
                std::cout << "куплен!";
                return;
            }
        }
    }

    std::cout << "Не куплен!\n";

    return;
}

void Client::showClient() {
    std::cout << "----------Информация----------\n";
    std::cout << "ФИО: " << this->name << "\n";
    std::cout << "Логин: " << this->user.getLogin() << "\n";
    std::cout << "Деньги: " << this->money << "\n";
}

void Client::toBalance() {
    try {
        int temp_money;

        std::cout << "Пожалуйста, введите количество денег для пополнения:\n";
        inputCheck(temp_money);

        if (temp_money > 0) {
            this->money += temp_money;
        }
        else {
            std::cout << "Сумма не может быть меньше или равной нулю!\n";
        }

        return;
    }
    catch (std::out_of_range ex) {
        std::cout << "Неккоректная сумма!\n";
        return;
    }
}