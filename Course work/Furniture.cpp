#define MAX_STRING_LENGTH 256
#include "Furniture.h"

std::ostream& operator<<(std::ostream& stream, Furniture& furniture) {
    if (!stream.bad() && !stream.fail()) {
        stream << furniture.name << "\n";
        stream << furniture.price << "\n";
        stream << furniture.amount << "\n";

        stream << furniture.components.size() << "\n";

        for (auto it = furniture.components.begin(); it != furniture.components.end(); it++) {
            stream << *it << "\n";
        }
    }

    return stream;
}

/*
std::ostream& operator<<(std::ostream& stream, Furniture& furniture) {
    if (stream.good()) {
        stream << furniture.name << "\n";
        stream << furniture.price << "\n";
        stream << furniture.amount << "\n";
        stream << furniture.components.size() << "\n";
        for (const auto& component : furniture.components) {
            stream << component << "\n";
        }
    }

    return stream;
}*/

std::istream& operator>>(std::istream& stream, Furniture& furniture) {
    char temp[MAX_STRING_LENGTH];
    int amount = 0;

    if (!stream.bad() && !stream.fail()) {
        stream.getline(temp, MAX_STRING_LENGTH);
        furniture.setName(temp);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);
        furniture.setPrice(amount);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);
        furniture.setAmount(amount);

        stream.getline(temp, MAX_STRING_LENGTH);
        amount = std::stoi(temp);

        std::list<std::string> temp_list;

        while (amount) {
            stream.getline(temp, MAX_STRING_LENGTH);

            temp_list.push_back(temp);
            amount--;
        }

        furniture.setComponents(temp_list);
    }

    return stream;
}

bool Furniture::operator>(const Furniture& furniture) {
    return this->name > furniture.name;
}

bool Furniture::operator<(const Furniture& furniture) {
    return this->name < furniture.name;
}

bool Furniture::operator==(const Furniture& furniture) {
    return this->name == furniture.name && this->price == furniture.price &&
        this->components == furniture.components;
}

Furniture::Furniture() {
    name = "NaN";
    price = 0;
    amount = 1;
}

Furniture::Furniture(std::string name, int price, int amount, std::list<std::string> components) {
    this->name = name;
    this->price = price;
    this->components = components;
    this->amount = amount;
}

Furniture::Furniture(const Furniture& furniture) {
    this->name = furniture.name;
    this->price = furniture.price;
    this->amount = furniture.amount;
    this->components = furniture.components;
}

std::string Furniture::getName() const{
    return name;
}

void Furniture::setName(std::string name) {
    this->name = name;
}

int Furniture::getPrice() const {
    return price;
}

void Furniture::setPrice(int price) {
    this->price = price;
}

int Furniture::getAmount() const {
    return amount;
}

void Furniture::setAmount(int number) {
    this->amount = number;
}

std::list<std::string> Furniture::getComponents() const {
    return components;
}

void Furniture::setComponents(std::list<std::string> components) {
    this->components = components;
}

void Furniture::addComponent(std::string component) {
    components.push_back(component);
}

void Furniture::deleteComponent(std::string component) {
    auto it = std::find(components.begin(), components.end(), component);

    if (it != components.end()) {
        components.erase(it);
    }
    else {
        std::cout << "Нет такого компонента!";
    }
}

void Furniture::showFurniture() {
    std::cout << std::endl;
    std::cout << "----------Информация----------\n";
    std::cout << "Название: " << this->name << "\n";
    std::cout << "Цена: " << this->price << "\n";
    std::cout << "Количество: " << this->amount << "\n";
    std::cout << "Компоненты:\n";

    for (auto it = this->components.begin(); it != this->components.end(); it++) {
        std::cout << *it << "   ";
    }
}