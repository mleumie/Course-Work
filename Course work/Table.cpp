#include "Table.h"

#include <vector>
#include <iostream>
#include <iomanip>


void Table::userTable(std::vector<Client>& clients) {
	for (auto i = clients.begin(); i != clients.end(); i++) {
		auto temp = i->getFurniture();
		auto start = temp.begin();

		std::cout << "ФИО: " << i->getName() << " | ";
		std::cout << "Логин: " << i->getUser().getLogin() << " | ";
		std::cout << "Деньги: " << i->getMoney() << " | ";

		if (temp.size() > 0) {
			std::cout << "Мебель: " << start->getName() << " | ";
			start++;
		}

		for (start; start != temp.end(); start++) {
			std::cout << " | ";
			std::cout << " | ";
			std::cout << " | ";
			std::cout << "Мебель: " << start->getName() << " | ";
		}

		std::cout << std::endl;
	}
}

void Table::adminTable(std::vector<Admin>& admins) {
	std::cout << std::endl;
	std::cout << std::setfill('-') << std::setw(120) << '-' << std::endl;
	std::cout << "|" << std::setfill(' ') << std::setw(65) << "Логины" << std::setw(54) << "|";
	std::cout << std::setfill('-') << std::setw(120) << '-' << std::endl;

	for (auto i = admins.begin(); i != admins.end(); i++) {
		std::cout << "|" << std::setfill(' ') << std::setw(65) << i->getLogin() << std::setw(54) << "|";
		std::cout << std::setfill('-') << std::setw(120) << '-' << std::endl;
	}
}

void Table::furnitureTable(std::vector<Furniture>& furnitures) {
	if (furnitures.empty()) {
		std::cout << "Нет доступной мебели.\n";
		return;
	}

	std::cout << "\n----------------------------------------------InteriorAlchemy-----------------------------------------------";
	std::cout << "\n+------------------------------+------------------+------------------+-------------------------------------+";
	std::cout << "\n|             Имя              |       Цена       |    Количество    |             Компоненты              |";
	std::cout << "\n+------------------------------+------------------+------------------+-------------------------------------+";

	for (const auto& furniture : furnitures) {
		std::cout << "\n" << std::setw(30) << furniture.getName()
			<< " | " << std::setw(16) << std::fixed << std::setprecision(2) << furniture.getPrice()
			<< " | " << std::setw(16) << furniture.getAmount();

		const auto components = furniture.getComponents();
		if (!components.empty()) {
			std::cout << " | ";
			for (auto it = components.begin(); it != components.end(); ++it) {
				if (it != components.begin()) {
					std::cout << ", ";
				}
				std::cout << *it;
			}
		}
		else {
			std::cout << " | " << std::setw(50) << "Нет компонентов";
		}

		std::cout << std::setw(0) << std::fixed << std::setprecision(6);
	}

	std::cout << "\n+------------------------------+------------------+------------------+-------------------------------------+\n";
}

void Table::furnitureTable(std::list<Furniture>& furnitures) {
	std::cout << "Список купленной мебели\n";
	std::cout << "-----------------------\n";
	for (auto i = furnitures.begin(); i != furnitures.end(); i++) {
		std::cout << i->getName() << "\n";
	}
}