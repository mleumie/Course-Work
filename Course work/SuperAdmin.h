#ifndef SUPERADMIN_H
#define SUPERADMIN_H

#include <iostream>
#include <string>

#include "Admin.h"

class SuperAdmin : public Admin {
private:
    static SuperAdmin* instance;
    SuperAdmin();
    SuperAdmin(const SuperAdmin&);
    SuperAdmin& operator= (SuperAdmin&);
    std::vector<Admin>::iterator findAdmin(std::string, std::vector<Admin>&, char);

public:
    friend std::ostream& operator<<(std::ostream&, SuperAdmin&);
    friend std::istream& operator>>(std::istream&, SuperAdmin&);

    static SuperAdmin* getInstance() {
        if (!instance) {
            instance = new SuperAdmin();
        }

        return instance;
    }

    SuperAdmin(std::string, std::string, std::string);

    Admin findAdmin(std::string, std::vector<Admin>&);
    static void sortAdmins(std::vector<Admin>&);
    void changeAdmin(std::string, std::vector<Admin>&);
    void deleteAdmin(std::string, std::vector<Admin>&);
    void addAdmin(std::vector<Admin>&);
};

#endif
