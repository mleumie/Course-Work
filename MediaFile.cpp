#include "MediaFile.h"
#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <iomanip>

namespace MediaShopNS {

// Static member initialization
int MediaFile::totalFilesCount = 0;
double MediaFile::totalSalesAmount = 0.0;

// MediaFile
MediaFile::MediaFile(std::string t, std::string a, double p, int s, std::string f)
    : title(std::move(t)), author(std::move(a)), format(std::move(f)), price(p), size(s) {
    ++totalFilesCount;
}

MediaFile::~MediaFile() {
    --totalFilesCount;
}

void MediaFile::edit() {
    std::cout << "Редактирование файла (Enter - оставить):\n";
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

    // Цена with error handling
    while (true) {
        std::cout << "Цена (" << price << "): ";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) break;
        try {
            double v = std::stod(input);
            if (v >= 0) { price = v; break; }
            else throw std::invalid_argument("Цена не может быть отрицательной");
        }
        catch (const std::invalid_argument& e) { 
            std::cout << "Ошибка: " << e.what() << "\n"; 
        }
        catch (const std::out_of_range&) { 
            std::cout << "Ошибка: число слишком большое\n"; 
        }
    }

    // Размер with error handling
    while (true) {
        std::cout << "Размер (" << size << "): ";
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) break;
        try {
            int v = std::stoi(input);
            if (v >= 0) { size = v; break; }
            else throw std::invalid_argument("Размер не может быть отрицательным");
        }
        catch (const std::invalid_argument& e) { 
            std::cout << "Ошибка: " << e.what() << "\n"; 
        }
        catch (const std::out_of_range&) { 
            std::cout << "Ошибка: число слишком большое\n"; 
        }
    }
}

std::string MediaFile::getType() const { return "Media"; }
double MediaFile::getPrice() const { return price; }
std::string MediaFile::getTitle() const { return title; }
std::string MediaFile::getAuthor() const { return author; }
std::string MediaFile::getFormat() const { return format; }
int MediaFile::getSize() const { return size; }

// Static methods implementation
int MediaFile::getTotalFilesCount() { return totalFilesCount; }
double MediaFile::getTotalSalesAmount() { return totalSalesAmount; }
void MediaFile::addSale(double amount) { totalSalesAmount += amount; }
void MediaFile::resetStatistics() { 
    totalFilesCount = 0; 
    totalSalesAmount = 0.0; 
}

// Serialize base class
std::string MediaFile::serialize() const {
    std::ostringstream oss;
    oss << getType() << "|" << title << "|" << author << "|" 
        << std::fixed << std::setprecision(2) << price << "|" << size << "|" << format;
    return oss.str();
}

// Friend operator<< overload for MediaFile
std::ostream& operator<<(std::ostream& os, const MediaFile& file) {
    os << "[" << file.getType() << "] " << file.title 
       << ", Автор: " << file.author 
       << ", Формат: " << file.format
       << ", Цена: " << std::fixed << std::setprecision(2) << file.price 
       << ", Размер: " << file.size << " МБ";
    return os;
}

// Friend operator>> overload for MediaFile (basic fields only)
std::istream& operator>>(std::istream& is, MediaFile& file) {
    std::cout << "Название: ";
    std::getline(is >> std::ws, file.title);
    std::cout << "Автор: ";
    std::getline(is >> std::ws, file.author);
    std::cout << "Формат: ";
    std::getline(is >> std::ws, file.format);
    std::cout << "Цена: ";
    is >> file.price;
    std::cout << "Размер (МБ): ";
    is >> file.size;
    return is;
}

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
            else throw std::invalid_argument("Длительность не может быть отрицательной");
        }
        catch (const std::invalid_argument& e) { 
            std::cout << "Ошибка: " << e.what() << "\n"; 
        }
        catch (const std::out_of_range&) { 
            std::cout << "Ошибка: число слишком большое\n"; 
        }
    }
    std::cout << "Альбом (" << album << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) album = input;
}

void AudioFile::printInfo() const {
    std::cout << "[Аудио] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Длительность: " << duration << " сек., Альбом: " << album << std::endl;
}

std::string AudioFile::getType() const { return "Audio"; }
std::string AudioFile::getAlbum() const { return album; }
int AudioFile::getDuration() const { return duration; }

std::string AudioFile::serialize() const {
    std::ostringstream oss;
    oss << MediaFile::serialize() << "|" << duration << "|" << album;
    return oss.str();
}

bool AudioFile::operator==(const AudioFile& other) const {
    return title == other.title && author == other.author && album == other.album;
}

bool AudioFile::operator<(const AudioFile& other) const {
    return price < other.price;
}

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
            else throw std::invalid_argument("Длительность не может быть отрицательной");
        }
        catch (const std::invalid_argument& e) { 
            std::cout << "Ошибка: " << e.what() << "\n"; 
        }
        catch (const std::out_of_range&) { 
            std::cout << "Ошибка: число слишком большое\n"; 
        }
    }
    std::cout << "Разрешение (" << resolution << "): ";
    std::getline(std::cin >> std::ws, input);
    if (!input.empty()) resolution = input;
}

void VideoFile::printInfo() const {
    std::cout << "[Видео] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Длительность: " << duration << " сек., Разрешение: " << resolution << std::endl;
}

std::string VideoFile::getType() const { return "Video"; }
std::string VideoFile::getResolution() const { return resolution; }
int VideoFile::getDuration() const { return duration; }

std::string VideoFile::serialize() const {
    std::ostringstream oss;
    oss << MediaFile::serialize() << "|" << duration << "|" << resolution;
    return oss.str();
}

bool VideoFile::operator==(const VideoFile& other) const {
    return title == other.title && author == other.author && resolution == other.resolution;
}

bool VideoFile::operator<(const VideoFile& other) const {
    return price < other.price;
}

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
    std::cout << "[Изображение] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, Разрешение: " << resolution << std::endl;
}

std::string ImageFile::getType() const { return "Image"; }
std::string ImageFile::getResolution() const { return resolution; }

std::string ImageFile::serialize() const {
    std::ostringstream oss;
    oss << MediaFile::serialize() << "|" << resolution;
    return oss.str();
}

bool ImageFile::operator==(const ImageFile& other) const {
    return title == other.title && author == other.author && resolution == other.resolution;
}

bool ImageFile::operator<(const ImageFile& other) const {
    return price < other.price;
}

} // namespace MediaShopNS