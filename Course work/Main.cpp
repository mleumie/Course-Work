#define SUPERADMIN "superadmin.txt"
#define ADMINS "admins.txt"
#define USERS "users.txt"
#define	FURNITURES "furnitures.txt"
#define DEFAULT "admin"

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <fstream>

#include "Transactions.h"
#include "SuperAdmin.h"
#include "Admin.h"
#include "User.h"
#include "Table.h"
#include "FileWork.h"
#include "Menu.h"


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::vector<Furniture> furnitures;
	std::vector<Admin> admins;
	std::vector<Client> clients;
	SuperAdmin super_admin(DEFAULT, DEFAULT, DEFAULT);

	inputFile(SUPERADMIN, ADMINS, USERS, FURNITURES, furnitures, clients, admins, super_admin);

	authMenu(furnitures, clients, admins, super_admin);

	outputFile(SUPERADMIN, ADMINS, USERS, FURNITURES, furnitures, clients, admins, super_admin);
}