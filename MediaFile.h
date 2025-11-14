#pragma once
#include <string>
#include <iostream>

// Базовый класс медиа-файла
class MediaFile {
protected:
    std::string title;
    std::string author;
    std::string format;
    double      price;
    int         size;
public:
    MediaFile(std::string t, std::string a, double p, int s, std::string f)
        : title(std::move(t)), author(std::move(a)), format(std::move(f)), price(p), size(s) {}

    virtual ~MediaFile() = default;

    virtual void printInfo() const = 0;

    virtual void edit() {
        std::cout << "Редактирование полей (Enter - оставить):\n";
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

        // Цена
        while (true) {
            std::cout << "Цена (" << price << "): ";
            std::getline(std::cin >> std::ws, input);
            if (input.empty()) break;
            try {
                double v = std::stod(input);
                if (v >= 0) { price = v; break; }
                else std::cout << "Нужно неотрицательное число\n";
            } catch (...) { std::cout << "Неправильный формат\n"; }
        }

        // Размер
        while (true) {
            std::cout << "Размер (" << size << "): ";
            std::getline(std::cin >> std::ws, input);
            if (input.empty()) break;
            try {
                int v = std::stoi(input);
                if (v >= 0) { size = v; break; }
                else std::cout << "Нужно неотрицательное число\n";
            } catch (...) { std::cout << "Неправильный формат\n"; }
        }
    }

    virtual std::string getType() const { return "Media"; }

    double      getPrice()  const { return price; }
    std::string getTitle()  const { return title; }
    std::string getAuthor() const { return author; }
    std::string getFormat() const { return format; }
    int         getSize()   const { return size; }
};

// AudioFile
class AudioFile : public MediaFile {
protected:
    int         duration;
    std::string album;
public:
    AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al)
        : MediaFile(std::move(t), std::move(a), p, s, std::move(f)), duration(d), album(std::move(al)) {}

    void edit() override {
        MediaFile::edit();
        std::string input;
        while (true) {
            std::cout << "Длительность (сек) (" << duration << "): ";
            std::getline(std::cin >> std::ws, input);
            if (input.empty()) break;
            try {
                int v = std::stoi(input);
                if (v >= 0) { duration = v; break; }
                else std::cout << "Нужно неотрицательное число\n";
            } catch (...) { std::cout << "Неправильный формат\n"; }
        }
        std::cout << "Альбом (" << album << "): ";
        std::getline(std::cin >> std::ws, input);
        if (!input.empty()) album = input;
    }

    void printInfo() const override {
        std::cout << "[Аудио] " << title << ", автор: " << author << ", формат: " << format
            << ", цена: " << price << ", размер: " << size << " Кб, длительность: " << duration
            << " сек, альбом: " << album << std::endl;
    }

    std::string getType() const override { return "Audio"; }
    std::string getAlbum() const { return album; }
    int getDuration() const { return duration; }
};

// VideoFile
class VideoFile : public MediaFile {
protected:
    int         duration;
    std::string resolution;
public:
    VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r)
        : MediaFile(std::move(t), std::move(a), p, s, std::move(f)), duration(d), resolution(std::move(r)) {}

    void edit() override {
        MediaFile::edit();
        std::string input;
        while (true) {
            std::cout << "Длительность (сек) (" << duration << "): ";
            std::getline(std::cin >> std::ws, input);
            if (input.empty()) break;
            try {
                int v = std::stoi(input);
                if (v >= 0) { duration = v; break; }
                else std::cout << "Нужно неотрицательное число\n";
            } catch (...) { std::cout << "Неправильный формат\n"; }
        }
        std::cout << "Разрешение (" << resolution << "): ";
        std::getline(std::cin >> std::ws, input);
        if (!input.empty()) resolution = input;
    }

    void printInfo() const override {
        std::cout << "[Видео] " << title << ", автор: " << author << ", формат: " << format
            << ", цена: " << price << ", размер: " << size << " Кб, длительность: " << duration
            << " сек, разрешение: " << resolution << std::endl;
    }

    std::string getType() const override { return "Video"; }
    std::string getResolution() const { return resolution; }
    int getDuration() const { return duration; }
};

// ImageFile
class ImageFile : public MediaFile {
protected:
    std::string resolution;
public:
    ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r)
        : MediaFile(std::move(t), std::move(a), p, s, std::move(f)), resolution(std::move(r)) {}

    void edit() override {
        MediaFile::edit();
        std::string input;
        std::cout << "Разрешение (" << resolution << "): ";
        std::getline(std::cin >> std::ws, input);
        if (!input.empty()) resolution = input;
    }

    void printInfo() const override {
        std::cout << "[Изображение] " << title << ", автор: " << author << ", формат: " << format
            << ", цена: " << price << ", размер: " << size << " Кб, разрешение: " << resolution << std::endl;
    }

    std::string getType() const override { return "Image"; }
    std::string getResolution() const { return resolution; }
};