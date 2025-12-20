#include <iostream>
#include <algorithm>
#include <iomanip>

#include "Menu.h"
#include "InputCheck.h"
#include "Validation.h"
#include "Client.h"
#include "StringGen.h"
#include "Table.h"
#include "Transactions.h"

void authMenu(std::vector<Furniture>& furnitures, std::vector<Client>& clients, std::vector<Admin>& admins, SuperAdmin& super_admin) {
	bool active = true;

	while (active) {
		std::string login = "", password = "";
		int counter;

		std::cout << std::left;
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "Выбор" << " | " << std::setw(30) << "Описание" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "1" << " | " << std::setw(30) << "Войти как пользователь" << " |\n";
		std::cout << "| " << std::setw(6) << "2" << " | " << std::setw(30) << "Войти как админ" << " |\n";
		std::cout << "| " << std::setw(6) << "3" << " | " << std::setw(30) << "Войти как суперадмин" << " |\n";
		std::cout << "| " << std::setw(6) << "4" << " | " << std::setw(30) << "Регистрация пользователя" << " |\n";
		std::cout << "| " << std::setw(6) << "5" << " | " << std::setw(30) << "Выход" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << std::resetiosflags(std::ios::adjustfield);

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			break;
		}
		case 1: {
			Validation::registration(login, password);
			int i = Validation::authUser(clients, login, password, role::user);

			if (i != clients.size()) {
				system("cls");
				clientMenu(clients.at(i), furnitures);
			}
			else {
				std::cout << "Нет такого пользователя!\n";
				system("pause");
				system("cls");
			}

			break;
		}
		case 2: {
			Validation::registration(login, password);
			int i = Validation::authAdmin(admins, login, password, role::admin);

			if (i != admins.size()) {
				system("cls");
				adminMenu(admins.at(i), furnitures, clients);
			}
			else {
				std::cout << "Нет такого администратора!\n";
				system("pause");
				system("cls");
			}

			break;
		}
		case 3: {
			Validation::registration(login, password);
			int i = Validation::authSuperAdmin(super_admin, login, password, role::superadmin);

			if (i) {
				system("cls");
				superAdminMenu(super_admin, furnitures, clients, admins);
			}
			else {
				std::cout << "Нет такого пользователя!\n";
				system("pause");
				system("cls");
			}

			break;
		}
		case 4: {
			auto it = clients.begin();
			std::string temp;

			Validation::registration(login, password);

			for (it; it != clients.end(); it++) {
				if (it->getUser().getLogin() == login) {
					break;
				}
			}

			if (it == clients.end()) {
				std::cout << "Пожалуйста, введите ФИО для своего аккаунта\n";
				getline(std::cin, temp);

				User temp_usr(login, password, StringGen::generate());
				Client cl(temp, temp_usr);

				std::cout << "Регистрация прошла успешно!\n";

				clients.push_back(cl);
			}
			else {
				std::cout << "Регистрация не завершена! Такой логин уже есть!\n";
			}


			system("pause");
			break;
		}
		case 5: {
			std::cout << "Осуществляется выход!\n";
			system("pause");
			active = false;
		}
		}

		system("cls");
	}
}

void clientMenu(Client& client, std::vector<Furniture>& furnitures) {
	bool active = true;

	while (active) {
		int counter;

		std::cout << std::left;
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "Выбор" << " | " << std::setw(30) << "Описание" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "1" << " | " << std::setw(30) << "Просмотр информации о клиенте" << " |\n";
		std::cout << "| " << std::setw(6) << "2" << " | " << std::setw(30) << "Посмотреть доступную мебель" << " |\n";
		std::cout << "| " << std::setw(6) << "3" << " | " << std::setw(30) << "Посмотреть свои покупки" << " |\n";
		std::cout << "| " << std::setw(6) << "4" << " | " << std::setw(30) << "Купить мебель" << " |\n";
		std::cout << "| " << std::setw(6) << "5" << " | " << std::setw(30) << "Пополнить баланс" << " |\n";
		std::cout << "| " << std::setw(6) << "6" << " | " << std::setw(30) << "Сменить логин" << " |\n";
		std::cout << "| " << std::setw(6) << "7" << " | " << std::setw(30) << "Сменить пароль" << " |\n";
		std::cout << "| " << std::setw(6) << "8" << " | " << std::setw(30) << "Выход" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << std::resetiosflags(std::ios::adjustfield);

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			system("pause");
			break;
		}
		case 1: {
			std::cout << std::endl;

			client.showClient();

			system("pause");
			break;
		}
		case 2: {
			Table::furnitureTable(furnitures);

			system("pause");
			break;
		}
		case 3: {
			auto temp = client.getFurniture();
			Table::furnitureTable(temp);
			system("pause");
			break;
		}
		case 4: {
			std::string temp_name;

			std::cout << "Пожалуйста, введите название мебели!\n";
			getline(std::cin, temp_name);

			client.buy(furnitures, temp_name);

			system("pause");
			break;
		}
		case 5: {
			client.toBalance();
			system("pause");
			break;
		}
		case 6: {
			std::string cont;
			std::string new_login;
			User temp = client.getUser(), prev = client.getUser();

			new_login = temp.changeLogin();
			Transaction<User> trans(&prev);

			temp.setLogin(new_login);

			if (!trans.startTransaction(&temp)) {
				std::cout << "Что-то пошло не так!\n";
			}

			std::cout << "Вы точно хотите продолжить? Yes/No\n";
			getline(std::cin, cont);

			if (cont == "Yes" || cont == "yes" || cont == "YES") {
				std::cout << "Операция продолжена!\n";
				client.setUser(trans.commitTransaction());
			}
			else {
				std::cout << "Операция отменена!\n";
				trans.denyTransaction();
			}

			break;
		}
		case 7: {
			std::string cont;
			std::string new_passwd;
			User temp = client.getUser(), prev = client.getUser();

			new_passwd = temp.changePassword();
			Transaction<User> trans(&prev);

			temp.setHash(new_passwd);

			if (!trans.startTransaction(&temp)) {
				std::cout << "Что-то пошло не так!\n";
			}

			std::cout << "Вы точно хотите продолжить? Yes/No\n";
			getline(std::cin, cont);

			if (cont == "Yes" || cont == "yes" || cont == "YES") {
				std::cout << "Операция продолжена!\n";
				client.setUser(trans.commitTransaction());
			}
			else {
				std::cout << "Операция отменена!\n";
				trans.denyTransaction();
			}
			break;
		}
		case 8: {
			std::cout << "Осуществляется выход!\n";
			system("pause");
			active = false;
		}
		}

		system("cls");
	}
}

void userWorkMenu(Admin* admin, std::vector<Client>& clients, std::vector<Furniture>& furnitures) {
	bool active = true;

	while (active) {
		int counter;

		std::cout << "Выберите пункт меню\n";
		std::cout << "1.Посмотреть всех пользователей\n";
		std::cout << "2.Сортировать пользователей\n";
		std::cout << "3.Найти пользователя\n";
		std::cout << "4.Удалить пользователя\n";
		std::cout << "5.Добавить пользователя\n";
		std::cout << "6.Выход\n";

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			system("pause");
			break;
		}
		case 1: {
			Table::userTable(clients);

			system("pause");
			break;
		}
		case 2: {
			admin->sortUsers(clients);

			system("pause");
			break;
		}
		case 3: {
			std::string temp_name;
			Client temp;

			std::cout << "Пожалуйста, введите логин для поиска!\n";
			getline(std::cin, temp_name);

			temp = admin->findUser(temp_name, clients);

			if (temp.getUser().getLogin() == "NONE") {
				std::cout << "Пользователь не был найден!\n";
			}
			else {
				temp.showClient();
			}

			system("pause");
			break;
		}
		case 4: {
			std::string temp_name;

			std::cout << "Пожалуйста, введите логин пользователя!\n";
			getline(std::cin, temp_name);

			admin->deleteUser(temp_name, clients);

			break;
		}
		case 5: {
			admin->addUser(clients);
			break;
		}
		case 6: {
			std::cout << "Осуществляется выход!\n";
			system("pause");
			active = false;
		}
		}

		system("cls");
	}
}

void adminMenu(Admin& admin, std::vector<Furniture>& furnitures, std::vector<Client>& clients) {
	bool active = true;

	while (active) {
		int counter;

		std::cout << std::left;
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "Выбор" << " | " << std::setw(30) << "Описание" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "1" << " | " << std::setw(30) << "Вывести информацию об админе" << " |\n";
		std::cout << "| " << std::setw(6) << "2" << " | " << std::setw(30) << "Меню работы с мебелью" << " |\n";
		std::cout << "| " << std::setw(6) << "3" << " | " << std::setw(30) << "Меню работы с пользователями" << " |\n";
		std::cout << "| " << std::setw(6) << "4" << " | " << std::setw(30) << "Редактирование учётной записи" << " |\n";
		std::cout << "| " << std::setw(6) << "5" << " | " << std::setw(30) << "Выход" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << std::resetiosflags(std::ios::adjustfield);

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			system("pause");
			break;
		}
		case 1: {
			admin.toScreen();
			system("pause");
			break;
		}
		case 2: {
			system("cls");
			furnitureWorkMenu(&admin, furnitures);

			system("pause");
			break;
		}
		case 3: {
			userWorkMenu(&admin, clients, furnitures);
			break;
		}
		case 4: {
			bool active = true;
			while (active) {
				std::cout << "1.Сменить логин\n";
				std::cout << "2.Сменить пароль\n";
				std::cout << "3.Выход\n";
				int choice;
				inputCheck(choice);
				switch (choice) {
				case 1:
				{
					std::string cont;
					std::string new_login;
					Admin temp = admin;

					new_login = temp.changeLogin();
					Transaction<Admin> p_admin(&admin);

					temp.setLogin(new_login);

					if (!p_admin.startTransaction(&temp)) {
						std::cout << "Что-то пошло не так!\n";
					}

					std::cout << "Вы точно хотите продолжить? Yes/No\n";
					getline(std::cin, cont);

					if (cont == "Yes" || cont == "yes" || cont == "YES") {
						std::cout << "Операция продолжена!\n";
						system("pause");
						admin = p_admin.commitTransaction();
					}
					else {
						std::cout << "Операция отменена!\n";
						p_admin.denyTransaction();
						system("pause");
					}
					break;
				}
				case 2:
				{
					std::string cont;
					std::string new_passwd;
					Admin temp = admin;

					new_passwd = admin.changePassword();
					Transaction<Admin> p_admin(&admin);

					temp.setHash(new_passwd);

					if (!p_admin.startTransaction(&temp)) {
						std::cout << "Что-то пошло не так!\n";
					}

					std::cout << "Вы точно хотите продолжить? Yes/No\n";
					getline(std::cin, cont);

					if (cont == "Yes" || cont == "yes" || cont == "YES") {
						std::cout << "Операция продолжена!\n";
						system("pause");
						admin = p_admin.commitTransaction();
					}
					else {
						std::cout << "Операция отменена!\n";
						p_admin.denyTransaction();
						system("pause");
					}
					break;
				}
				case 3:
				{
					std::cout << "Осуществляется выход!\n";
					system("pause");
					active = false;
				}
				system("cls");
				}
			}
			break;
		}
		case 5: {
			std::cout << "Осуществляется выход!\n";
			system("pause");
			active = false;
		}
		}
		system("cls");
	}
}

void furnitureWorkMenu(Admin* admin, std::vector<Furniture>& furnitures) {
	bool active = true;

	while (active) {
		int counter;

		std::cout << "Выберите пункт меню\n";
		std::cout << "1.Посмотреть список мебели\n";
		std::cout << "2.Сортировать мебель\n";
		std::cout << "3.Найти нужную мебель\n";
		std::cout << "4.Изменить параметры мебели\n";
		std::cout << "5.Удалить мебель\n";
		std::cout << "6.Добавить мебель в список\n";
		std::cout << "7.Выход\n";

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			system("pause");
			break;
		}
		case 1: {
			Table::furnitureTable(furnitures);

			system("pause");
			break;
		}
		case 2: {
			admin->sortFurniture(furnitures);

			system("pause");
			break;
		}
		case 3: {
			std::string temp_name;
			Furniture temp;

			std::cout << "Пожалуйста, введите название мебели!\n";
			getline(std::cin, temp_name);

			temp = admin->findFurniture(temp_name, furnitures);

			if (temp.getName() == "NaN") {
				std::cout << "Нужная мебель не была найдена!\n";
			}
			else {
				temp.showFurniture();
			}
			std::cout << std::endl;
			system("pause");
			break;
		}
		case 4: {
			std::string temp_name;
			Furniture temp;

			std::cout << "Пожалуйста, введите название мебели для редоктирования!\n";
			getline(std::cin, temp_name);

			admin->changeFurniture(temp_name, furnitures);

			break;
		}
		case 5: {
			std::string temp_name;

			std::cout << "Пожалуйста, введите название мебели, которую нужно удалить!\n";
			getline(std::cin, temp_name);

			admin->deleteFurniture(temp_name, furnitures);

			break;
		}
		case 6: {
			admin->addFurniture(furnitures);

			break;
		}
		case 7: {
			std::cout << "Осуществляется выход!\n";
			system("pause");
			active = false;
		}
		}

		system("cls");
	}
}


void adminWorkMenu(SuperAdmin& admin, std::vector<Admin>& admins) {
	bool active = true;

	while (active) {
		int counter;

		std::cout << "Выберите пункт меню\n";
		std::cout << "1.Посмотреть всех администраторов\n";
		std::cout << "2.Сортировать администраторов\n";
		std::cout << "3.Найти администратора\n";
		std::cout << "4.Изменить администратора\n";
		std::cout << "5.Удалить администратора\n";
		std::cout << "6.Добавить администратора\n";
		std::cout << "7.Выход\n";

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			system("pause");
			break;
		}
		case 1: {
			Table::adminTable(admins);

			system("pause");
			break;
		}
		case 2: {
			admin.sortAdmins(admins);

			system("pause");
			break;
		}
		case 3: {
			std::string login;
			Admin temp;

			std::cout << "Пожалуйста, введите логин администратора!\n";
			getline(std::cin, login);

			temp = admin.findAdmin(login, admins);

			if (temp.getLogin() == "Admin") {
				std::cout << "Администратор не был найден!\n";
			}
			else {
				std::cout << "Такой администратор существует!\n";
			}

			system("pause");
			break;
		}
		case 4: {
			std::string login;

			std::cout << "Пожалуйста, введите логин администратора!\n";
			getline(std::cin, login);

			admin.changeAdmin(login, admins);

			break;
		}
		case 5: {
			std::string temp_name;

			std::cout << "Пожалуйста, введите логин администратора!\n";
			getline(std::cin, temp_name);

			admin.deleteAdmin(temp_name, admins);

			break;
		}
		case 6: {
			std::string temp_name;

			admin.addAdmin(admins);

			break;
		}
		case 7: {
			std::cout << "Осуществляется выход!\n";
			system("pause");
			active = false;
		}
		}

		system("cls");
	}
}

void superAdminMenu(SuperAdmin& admin, std::vector<Furniture>& furnitures,
	std::vector<Client>& clients, std::vector<Admin>& admins) {
	bool active = true;

	while (active) {
		int counter;

		std::cout << std::left;
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "Выбор" << " | " << std::setw(30) << "Описание" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << "| " << std::setw(6) << "1" << " | " << std::setw(30) << "Просмотр информации об админе" << " |\n";
		std::cout << "| " << std::setw(6) << "2" << " | " << std::setw(30) << "Меню работы с мебелью" << " |\n";
		std::cout << "| " << std::setw(6) << "3" << " | " << std::setw(30) << "Меню работы с пользователями" << " |\n";
		std::cout << "| " << std::setw(6) << "4" << " | " << std::setw(30) << "Меню работы с админами" << " |\n";
		std::cout << "| " << std::setw(6) << "5" << " | " << std::setw(30) << "Сменить логин" << " |\n";
		std::cout << "| " << std::setw(6) << "6" << " | " << std::setw(30) << "Сменить пароль" << " |\n";
		std::cout << "| " << std::setw(6) << "7" << " | " << std::setw(30) << "Выход" << " |\n";
		std::cout << std::setfill('=') << std::setw(43) << "" << std::setfill(' ') << std::endl;
		std::cout << std::resetiosflags(std::ios::adjustfield);

		inputCheck(counter);

		switch (counter) {
		default: {
			std::cout << "Некорректный ввод\n";
			system("pause");
			break;
		}
		case 1: {
			std::cout << std::endl;

			std::cout << "Информация:\n";
			std::cout << "Логин: " << admin.getLogin() << "\n";

			system("pause");
			break;
		}
		case 2: {
			system("cls");
			furnitureWorkMenu(&admin, furnitures);

			system("pause");
			break;
		}
		case 3: {
			system("cls");
			userWorkMenu(&admin, clients, furnitures);

			system("pause");
			break;
		}
		case 4: {
			system("cls");
			adminWorkMenu(admin, admins);

			system("pause");
			break;
		}
		case 5: {
			std::string cont;
			std::string new_login;

			new_login = admin.changeLogin();

			std::cout << "Вы точно хотите продолжить? Yes/No\n";
			getline(std::cin, cont);

			if (cont == "Yes" || cont == "yes" || cont == "YES") {
				std::cout << "Операция продолжена!\n";
				admin.setLogin(new_login);
			}
			else {
				std::cout << "Операция отменена!\n";
			}

			break;
		}
		case 6: {
			std::string cont;
			std::string new_passwd;

			new_passwd = admin.changePassword();

			std::cout << "Вы точно хотите продолжить? Yes/No\n";
			getline(std::cin, cont);

			if (cont == "Yes" || cont == "yes" || cont == "YES") {
				std::cout << "Операция продолжена!\n";
				admin.setHash(new_passwd);
			}
			else {
				std::cout << "Операция отменена!\n";
			}

			break;
		}
		case 7: {
			std::cout << "Осуществляется выход!\n";
		
			system("pause");
			active = false;
		}
		}

		system("cls");
	}
}