#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include "InputCheck.h"

void inputCheck(int& res) {
    std::string value;
    std::stringstream stream;

    stream.str("");

    std::cout << "Введите целое число\n";

    std::cin >> value;
    stream << value;
    stream >> res;
    stream.str("");
    stream.clear();

    while (!res) {
        std::cout << "Некорректный ввод\n";

        std::cin >> value;
        stream << value;
        stream >> res;
        stream.str("");
        stream.clear();
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}