#include <iostream>
#include <ctime>
#include <string>

#include "StringGen.h"

#define STR_LENGTH 16

std::string StringGen::generate() {
    static const char alphanum[] =
        "01234567890"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "!@#$%^&*()_-+=";

    std::string temp_str;
    temp_str.reserve(STR_LENGTH);

    for (int i = 0; i < STR_LENGTH; i++) {
        temp_str += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return temp_str;
}
