#ifndef TABLE_H
#define TABLE_H

#include "Furniture.h"
#include "Client.h"
#include "Admin.h"

#include <vector>

class Table {
public:
	static void userTable(std::vector<Client>&);
	static void adminTable(std::vector<Admin>&);
	static void furnitureTable(std::vector<Furniture>&);
	static void furnitureTable(std::list<Furniture>&);
};

#endif