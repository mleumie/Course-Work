#include "ConsoleUI.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

// Очищает экран (Windows)
static void clearScreen() {
    system("cls");
}

// Установить атрибут консоли
static void setConsoleAttr(WORD attr) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, attr);
}

// Печать аккуратного заголовка (ширина 80, рамка из '=')
void printHeader(const std::string& title) {
    const int width = 80;
    int pad = (width - static_cast<int>(title.size())) / 2;
    if (pad < 0) pad = 0;
    clearScreen();
    std::cout << std::string(width, '=') << "\n"
        << std::string(pad, ' ') << title << "\n"
        << std::string(width, '=') << "\n\n";
}

// Интерактивное меню строк (стрелки и подсветка)
int MenuStrings(const std::vector<std::string>& items, const std::string& title) {
    if (items.empty()) return -1;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const WORD attrNormal = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    const WORD attrHighlight = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    int pos = 0;
    int n = static_cast<int>(items.size());
    while (true) {
        clearScreen();
        printHeader(title);
        for (int i = 0; i < n; ++i) {
            if (i == pos) SetConsoleTextAttribute(hConsole, attrHighlight);
            else SetConsoleTextAttribute(hConsole, attrNormal);
            std::cout << (i + 1) << ". " << items[i] << "\n";
        }
        SetConsoleTextAttribute(hConsole, attrNormal);
        int c = _getch();
        if (c == 0 || c == 224) {
            c = _getch();
            if (c == 72) pos = (pos - 1 + n) % n;         // стрелка вверх
            else if (c == 80) pos = (pos + 1) % n;        // стрелка вниз
        }
        else if (c == 13) {
            return pos;                                   // Enter
        }
        else if (c == 27) {
            return -1;                                    // Esc
        }
    }
}

// Ожидание любой клавиши
void getInput() {
    std::cout << "\nНажмите любую клавишу...";
    _getch();
}