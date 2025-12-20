#ifndef MENU_H
#define MENU_H

#include <vector>

#include "User.h"
#include "Client.h"
#include "Admin.h"
#include "SuperAdmin.h"

void authMenu(std::vector<Furniture>&, std::vector<Client>&, std::vector<Admin>&, SuperAdmin&);
void clientMenu(Client&, std::vector<Furniture>&);
void adminMenu(Admin&, std::vector<Furniture>&, std::vector<Client>&);
void superAdminMenu(SuperAdmin&, std::vector<Furniture>&, std::vector<Client>&, std::vector<Admin>&);

void furnitureWorkMenu(Admin*, std::vector<Furniture>&);
void userWorkMenu(Admin*, std::vector<Client>&, std::vector<Furniture>&);
void adminWorkMenu(SuperAdmin&, std::vector<Admin>&);

#endif
