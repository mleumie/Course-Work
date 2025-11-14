#pragma once
#include <string>
#include <iostream>

class MediaFile {
protected:
    std::string title;
    std::string author;
    std::string format;
    double price;
    int size;
public:
    MediaFile(std::string t, std::string a, double p, int s, std::string f);
    virtual void printInfo() const = 0;
    virtual ~MediaFile();
    double getPrice() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getFormat() const;
    int getSize() const;
};

class AudioFile : public MediaFile {
protected:
    int duration;
    std::string album;
public:
    AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al);
    void printInfo() const override;
};

class VideoFile : public MediaFile {
protected:
    int duration;
    std::string resolution;
public:
    VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r);
    void printInfo() const override;
};

class ImageFile : public MediaFile {
protected:
    std::string resolution;
public:
    ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r);
    void printInfo() const override;
};