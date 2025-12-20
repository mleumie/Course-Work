#ifndef FURNITURE_H
#define FURNITURE_H

#include <iostream>
#include <string>
#include <list>

class Furniture {
private:
    std::string name;
    int price;
    int amount;
    std::list<std::string> components;

public:
    friend std::ostream& operator<<(std::ostream&, Furniture&);
    friend std::istream& operator>>(std::istream&, Furniture&);

    bool operator>(const Furniture&);
    bool operator<(const Furniture&);
    bool operator==(const Furniture&);

    Furniture();
    Furniture(std::string, int, int, std::list<std::string>);
    Furniture(const Furniture&);

    std::string getName() const;
    void setName(std::string);

    int getPrice() const;
    void setPrice(int);

    int getAmount() const;
    void setAmount(int);

    std::list<std::string> getComponents() const;
    void setComponents(std::list<std::string>);

    void addComponent(std::string);
    void deleteComponent(std::string);

    void showFurniture();
};

#endif
