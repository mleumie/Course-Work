#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <string>

#include "Roles.h"
#include "Admin.h"
#include "Client.h"
#include "superAdmin.h"

class Validation {
public:
    static void registration(std::string&, std::string&);
    static int authUser(std::vector<Client>, std::string, std::string, role);
    static int authAdmin(std::vector<Admin>, std::string, std::string, role);
    static int authSuperAdmin(SuperAdmin&, std::string, std::string, role);
};

#endif
