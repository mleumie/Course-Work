#include "MediaFile.h"
#include <iostream>

MediaFile::MediaFile(std::string t, std::string a, double p, int s, std::string f)
    : title(t), author(a), price(p), size(s), format(f) {
}
MediaFile::~MediaFile() {}
double MediaFile::getPrice() const { return price; }
std::string MediaFile::getTitle() const { return title; }
std::string MediaFile::getAuthor() const { return author; }
std::string MediaFile::getFormat() const { return format; }
int MediaFile::getSize() const { return size; }

// AudioFile
AudioFile::AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al)
    : MediaFile(t, a, p, s, f), duration(d), album(al) {
}
void AudioFile::printInfo() const {
    std::cout << "[Аудио] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Длительность: " << duration << " сек., Альбом: " << album << std::endl;
}

// VideoFile
VideoFile::VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r)
    : MediaFile(t, a, p, s, f), duration(d), resolution(r) {
}
void VideoFile::printInfo() const {
    std::cout << "[Видео] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Длительность: " << duration << " сек., Разрешение: " << resolution << std::endl;
}

// ImageFile
ImageFile::ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r)
    : MediaFile(t, a, p, s, f), resolution(r) {
}
void ImageFile::printInfo() const {
    std::cout << "[Изображение] " << title << ", Автор: " << author << ", Формат: " << format
        << ", Цена: " << price << ", Размер: " << size << " МБ, "
        << "Разрешение: " << resolution << std::endl;
}