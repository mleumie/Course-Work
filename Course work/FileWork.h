#ifndef FILEWORK_H
#define FILEWORK_H

#include <iostream>
#include <vector>
#include <string>

#include "Furniture.h"
#include "Client.h"
#include "Admin.h"
#include "SuperAdmin.h"

void inputFile(std::string, std::string, std::string, std::string,
	std::vector<Furniture>&, std::vector<Client>&, std::vector<Admin>&,
	SuperAdmin&);

void outputFile(std::string, std::string, std::string, std::string,
	std::vector<Furniture>&, std::vector<Client>&, std::vector<Admin>&,
	SuperAdmin&);

#endif
