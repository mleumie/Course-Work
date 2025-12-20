#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <list>
#include <vector>

#include "Furniture.h"
#include "User.h"

class Client {
private:
    std::string name;
    int money;
    std::list<Furniture> furnitures;
    User user;

public:
    void addFurniture(Furniture);

    friend std::ostream& operator<<(std::ostream&, Client&);
    friend std::istream& operator>>(std::istream&, Client&);

    bool operator<(const Client&);
    bool operator>(const Client&);
    bool operator==(const Client&);

    Client(std::string name = "client", User usr = User(), int money = 0,
        std::list<Furniture> furnitures = std::list<Furniture>()) : name(name),
        user(usr), money(money), furnitures(furnitures) {}

    std::string getName();
    void setName(std::string);

    std::list<Furniture> getFurniture();
    void setFurniture(std::list<Furniture>);

    int getMoney();
    void setMoney(int);

    Furniture getFurniture(int);
    Furniture getFurniture(std::string);

    User getUser();
    void setUser(User);

    void buy(std::vector<Furniture>&, std::string);

    void showClient();
    void toBalance();
};

#endif
