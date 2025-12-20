#include "FileWork.h"

#include <fstream>

void inputFile(std::string SUPERADMIN, std::string ADMINS, std::string USERS, std::string FURNITURES,
	std::vector<Furniture>& furnitures, std::vector<Client>& clients, std::vector<Admin>& admins, SuperAdmin& super_admin) {
	try {
		std::fstream fusers, fadmins, fsuperadmin, ffurnitures;

		fusers.open(USERS, std::fstream::in);
		fadmins.open(ADMINS, std::fstream::in);
		fsuperadmin.open(SUPERADMIN, std::fstream::in);
		ffurnitures.open(FURNITURES, std::fstream::in);


		if (fusers.is_open()) {
			std::string temp;
			int amount;

			getline(fusers, temp);
			amount = std::stoi(temp);

			for (amount; amount > 0; amount--) {
				Client temp;
				fusers >> temp;
				clients.push_back(temp);
			}
		}

		if (fadmins.is_open()) {
			std::string temp;
			int amount;

			getline(fadmins, temp);
			amount = std::stoi(temp);

			for (amount; amount > 0; amount--) {
				Admin temp;
				fadmins >> temp;
				admins.push_back(temp);
			}
		}

		if (fsuperadmin.is_open()) {
			fsuperadmin >> super_admin;
		}

		if (ffurnitures.is_open()) {
			std::string temp;
			int amount;

			getline(ffurnitures, temp);
			amount = std::stoi(temp);

			for (amount; amount > 0; amount--) {
				Furniture temp;
				ffurnitures >> temp;
				furnitures.push_back(temp);
			}
		}

		fusers.close();
		fadmins.close();
		fsuperadmin.close();
		ffurnitures.close();
	}
	catch (...) {
		std::cout << "Запись из файла не произошла!\n";
		system("cls");
	}
}

void outputFile(std::string SUPERADMIN, std::string ADMINS, std::string USERS, std::string FURNITURES,
	std::vector<Furniture>& furnitures, std::vector<Client>& clients, std::vector<Admin>& admins, SuperAdmin& super_admin) {
	try {
		std::fstream fusers, fadmins, fsuperadmin, ffurnitures;

		fusers.open(USERS, std::fstream::out);
		fadmins.open(ADMINS, std::fstream::out);
		fsuperadmin.open(SUPERADMIN, std::fstream::out);
		ffurnitures.open(FURNITURES, std::fstream::out);

		if (fusers.is_open()) {
			fusers << clients.size() << "\n";

			for (int i = 0; i < clients.size(); i++) {
				fusers << clients.at(i);
			}
		}

		if (fadmins.is_open()) {
			fadmins << admins.size() << "\n";

			for (int i = 0; i < admins.size(); i++) {
				fadmins << admins.at(i);
			}
		}

		if (fsuperadmin.is_open()) {
			fsuperadmin << super_admin;
		}

		if (ffurnitures.is_open()) {
			ffurnitures << furnitures.size() << "\n";

			for (int i = 0; i < furnitures.size(); i++) {
				ffurnitures << furnitures.at(i);
			}
		}

		fusers.close();
		fadmins.close();
		fsuperadmin.close();
		ffurnitures.close();
	}
	catch (...) {
		std::cout << "Запись в файл не произошла!\n";
		system("cls");
	}

}