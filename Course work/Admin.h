#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "User.h"
#include "Furniture.h"
#include "Client.h"

class Admin : public User {
private:
    std::vector<Furniture>::iterator findFurniture(std::string, std::vector<Furniture>&, char);
    std::vector<Client>::iterator findUser(std::string, std::vector<Client>&, char);
    std::list<Furniture>::iterator findFurniture(std::string, std::list<Furniture>&, char);

public:
    friend std::ostream& operator<<(std::ostream&, Admin&);
    friend std::istream& operator>>(std::istream&, Admin&);

    bool operator<(const Admin&);
    bool operator>(const Admin&);
    bool operator==(const Admin&);

    Admin();
    Admin(std::string, std::string, std::string);
    Admin(const Admin&);

    static void sortUsers(std::vector<Client>&);
    Client findUser(std::string, std::vector<Client>&);
    void changeUser(std::string, std::vector<Client>&, std::vector<Furniture>&);
    void deleteUser(std::string, std::vector<Client>&);
    void addUser(std::vector<Client>&);
     
    static void sortFurniture(std::vector<Furniture>&);
    Furniture findFurniture(std::string, std::vector<Furniture>&);
    void changeFurniture(std::string, std::vector<Furniture>&);
    void deleteFurniture(std::string, std::vector<Furniture>&);
    void addFurniture(std::vector<Furniture>&);
};

#endif
