#pragma once
// ConsoleUI: простое цветное меню для Windows.
// Показывает список, выделяет текущую строку цветом (SetConsoleTextAttribute).
// Поддерживает стрелки вверх/вниз, Enter — выбрать, Esc — отмена.

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>

inline void clearScreenSimple() { system("cls"); }

inline void printHeader(const std::string& title) {
    const int width = 80;
    int pad = (width - static_cast<int>(title.size())) / 2;
    if (pad < 0) pad = 0;
    clearScreenSimple();
    std::cout << std::string(width, '=') << "\n"
        << std::string(pad, ' ') << title << "\n"
        << std::string(width, '=') << "\n\n";
}

// Цветовое выделение: normal и highlight
inline void printMenuWithColors(const std::vector<std::string>& items, int pos) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const WORD attrNormal = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // обычный (серый/белый)
    const WORD attrHighlight = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // выделение

    for (int i = 0; i < (int)items.size(); ++i) {
        if (i == pos) SetConsoleTextAttribute(hConsole, attrHighlight);
        else SetConsoleTextAttribute(hConsole, attrNormal);
        std::cout << "  " << items[i] << "\n";
    }
    SetConsoleTextAttribute(hConsole, attrNormal);
}

// Интерактивное меню: возвращает индекс выбранного пункта или -1 при Esc
inline int MenuStrings(const std::vector<std::string>& items, const std::string& title = "") {
    if (items.empty()) return -1;
    int pos = 0;
    while (true) {
        printHeader(title);
        printMenuWithColors(items, pos);
        int c = _getch();
        if (c == 0 || c == 224) {
            c = _getch();
            if (c == 72) { if (pos > 0) --pos; }           // вверх
            else if (c == 80) { if (pos < (int)items.size() - 1) ++pos; } // вниз
        }
        else if (c == 13) {
            return pos;
        }
        else if (c == 27) {
            return -1;
        }
    }
}

// Пауза — ждём любую клавишу (без подсказок)
inline void getInput() {
    _getch();
}
