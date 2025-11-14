#include "MediaFile.h"
#include <iostream>

// MediaFile
MediaFile::MediaFile(std::string t, std::string a, double p, int s, std::string f)
    : title(t), author(a), price(p), size(s), format(f) {
}
MediaFile::~MediaFile() {}
int MediaFile::getSize()           const { return size; }
double MediaFile::getPrice()       const { return price; }
std::string MediaFile::getTitle()  const { return title; }
std::string MediaFile::getAuthor() const { return author; }
std::string MediaFile::getFormat() const { return format; }
std::string MediaFile::getType()   const { return "Media"; }

void MediaFile::edit() {
    std::cout << "Редактирование файла (Enter - пропустить ввод)\n";
    std::string input;
    std::cout << "Изменить название (" << title << "):";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) title = input;

    std::cout << "Изменить автора (" << author << "):";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) author = input;

    std::cout << "Изменить формат (" << format << "):";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) format = input;

    bool valid = false;
    while (!valid) {
        std::cout << "Изменить цену (" << price << "):";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) { break; }
        try { 
            double newPrice = std::stod(input);
            if (newPrice >= 0) {
                price = newPrice;
                valid = true;
            }
            else {
                std::cout << "Цена не может быть отрицательной\n";
            }
        }
        catch (...) { std::cout << "Неверный формат числа\n"; }
    }

    valid = false;
    while (!valid) {
        std::cout << "Изменить размер (" << size << "):";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) { break; }
        try { 
            int newSize = std::stoi(input);
            if (newSize >= 0) {
                size = newSize;
                valid = true;
            }
            else {
                std::cout << "Размер не может быть отрицательным\n";
            }
        }
        catch (...) { std::cout << "Неверный формат числа\n"; }
    }
}

// AudioFile
AudioFile::AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al)
    : MediaFile(t, a, p, s, f), duration(d), album(al) {
}
std::string AudioFile::getType() const { return "Audio"; }
int AudioFile::getDuration() const { return duration; }
std::string AudioFile::getAlbum() const { return album; }

void AudioFile::printInfo() const {
    std::cout << "[Аудио] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Длительность: " << duration << " сек., Альбом: " << album << std::endl;
}

void AudioFile::edit() {
    MediaFile::edit();
    std::string input;
    bool valid = false;
    while (!valid) {
    std::cout << "Изменить продолжительность (сек.) (" << duration << "):";
    std::getline(std::cin >> std::ws, input);
        if (input.empty()) { break; }
        try {
            int newDur = std::stoi(input);
            if (newDur >= 0) {
                duration = newDur;
                valid = true;
            }
            else {
                std::cout << "Продолжительность не может быть отрицательной\n";
            }
        }
        catch (...) { std::cout << "Неверный формат числа\n"; }
    }

    std::cout << "Изменить альбом (" << album << "):";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) album = input;
}

// VideoFile
VideoFile::VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r)
    : MediaFile(t, a, p, s, f), duration(d), resolution(r) {
}
std::string VideoFile::getType() const { return "Video"; }
int VideoFile::getDuration() const { return duration; }
std::string VideoFile::getResolution() const { return resolution; }

void VideoFile::printInfo() const {
    std::cout << "[Видео] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Длительность: " << duration << " сек., Разрешение: " << resolution << std::endl;
}

void VideoFile::edit() {
    MediaFile::edit();
    std::string input;
    bool valid = false;
    while (!valid) {
        std::cout << "Изменить продолжительность (сек.) (" << duration << "):";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) { break; }
        try {
            int newDur = std::stoi(input);
            if (newDur >= 0) {
                duration = newDur;
                valid = true;
            }
            else {
                std::cout << "Продолжительность не может быть отрицательной\n";
            }
        }
        catch (...) { std::cout << "Неверный формат числа\n"; }
    }

    std::cout << "Изменить качество (" << resolution << "):";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) resolution = input;
}

// ImageFile
ImageFile::ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r)
    : MediaFile(t, a, p, s, f), resolution(r) {
}
std::string ImageFile::getType() const { return "Image"; }
std::string ImageFile::getResolution() const { return resolution; }

void ImageFile::printInfo() const {
    std::cout << "[Изображение] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Разрешение: " << resolution << std::endl;
}

void ImageFile::edit() {
    MediaFile::edit();
    std::string input;
    std::cout << "Изменить качество (" << resolution << "):";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) resolution = input;
}