#include "MediaFile.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>

// MediaFile
MediaFile::MediaFile(std::string t, std::string a, double p, int s, std::string f)
    : title(t), author(a), format(f), price(p), size(s) {
}
MediaFile::~MediaFile() {};
void MediaFile::edit() {
    std::cout << "Редактирование файла:\n";
    std::string input;

    std::cout << "Название (" << title << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) title = input;

    std::cout << "Автор (" << author << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) author = input;

    std::cout << "Формат (" << format << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) format = input;

    while (true) {
        std::cout << "Цена (" << price << "): ";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) break;
        try {
            double v = std::stod(input);
            if (v >= 0) { price = v; break; }
            else std::cout << "Цена должна быть неотрицательной\n";
        }
        catch (...) { std::cout << "Некорректный ввод цены\n"; }
    }

    while (true) {
        std::cout << "Размер (" << size << "): ";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) break;
        try {
            int v = std::stoi(input);
            if (v >= 0) { size = v; break; }
            else std::cout << "Размер должен быть неотрицательным\n";
        }
        catch (...) { std::cout << "Некорректный ввод размера\n"; }
    }
}

std::string MediaFile::getType() const { return "Media"; }
double MediaFile::getPrice() const { return price; }
std::string MediaFile::getTitle() const { return title; }
std::string MediaFile::getAuthor() const { return author; }
std::string MediaFile::getFormat() const { return format; }
int MediaFile::getSize() const { return size; }

// AudioFile
AudioFile::AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al)
    : MediaFile(std::move(t), std::move(a), p, s, std::move(f)), duration(d), album(std::move(al)) {
}

void AudioFile::edit() {
    MediaFile::edit();
    std::string input;
    while (true) {
        std::cout << "Длительность (сек) (" << duration << "): ";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) break;
        try {
            int v = std::stoi(input);
            if (v >= 0) { duration = v; break; }
            else std::cout << "Длительность должна быть неотрицательной\n";
        }
        catch (...) { std::cout << "Некорректный ввод\n"; }
    }
    std::cout << "Альбом (" << album << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) album = input;
}

void AudioFile::printInfo() const {
    std::cout << "[Аудио] " << title << " | автор: " << author << "| формат: " << format
        << " | цена: " << price << " | размер: " << size << " кБ | "
        << "длительность: " << duration << " сек. | альбом: " << album << std::endl;
}

std::string AudioFile::getType() const { return "Audio"; }
std::string AudioFile::getAlbum() const { return album; }
int AudioFile::getDuration() const { return duration; }

// VideoFile
VideoFile::VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r)
    : MediaFile(std::move(t), std::move(a), p, s, std::move(f)), duration(d), resolution(std::move(r)) {
}

void VideoFile::edit() {
    MediaFile::edit();
    std::string input;
    while (true) {
        std::cout << "Длительность (сек) (" << duration << "): ";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) break;
        try {
            int v = std::stoi(input);
            if (v >= 0) { duration = v; break; }
            else std::cout << "Длительность должна быть неотрицательной\n";
        }
        catch (...) { std::cout << "Некорректный ввод\n"; }
    }
    std::cout << "Разрешение (" << resolution << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) resolution = input;
}

void VideoFile::printInfo() const {
    std::cout << "[Видео] " << title << " | автор: " << author << " | формат: " << format
        << " | цена: " << price << " | размер: " << size << " кБ | "
        << "длительность: " << duration << " сек. | разрешение: " << resolution << std::endl;
}

std::string VideoFile::getType() const { return "Video"; }
std::string VideoFile::getResolution() const { return resolution; }
int VideoFile::getDuration() const { return duration; }

// ImageFile
ImageFile::ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r)
    : MediaFile(std::move(t), std::move(a), p, s, std::move(f)), resolution(std::move(r)) {
}

void ImageFile::edit() {
    MediaFile::edit();
    std::string input;
    std::cout << "Разрешение (" << resolution << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) resolution = input;
}

void ImageFile::printInfo() const {
    std::cout << "[Изображение] " << title << ", автор: " << author << ", формат: " << format
        << ", цена: " << price << ", размер: " << size << " кБ, разрешение: " << resolution << std::endl;
}

std::string ImageFile::getType() const { return "Image"; }
std::string ImageFile::getResolution() const { return resolution; }

std::ostream& operator<<(std::ostream& os, const MediaFile& f) {
    os << f.getType() << " \"" << f.title << "\"\n"
        << "Автор: " << f.author << "\n"
        << "Формат: " << f.format << "\n"
        << "Цена: " << std::fixed << std::setprecision(2) << f.price << "\n"
        << "Размер: " << f.size << " кБ";
    return os;
}

bool operator<(const MediaFile& a, const MediaFile& b) {
    if (a.price != b.price) return a.price < b.price;
    if (a.title != b.title) return a.title < b.title;
    if (a.author != b.author) return a.author < b.author;
    return a.format < b.format;
}