#pragma once
#include <string>

// Базовый класс медиа-файла
class MediaFile {
protected:
    std::string title;
    std::string author;
    std::string format;
    double      price;
    int         size;
public:
    MediaFile(std::string t, std::string a, double p, int s, std::string f);
    virtual ~MediaFile();

    virtual void printInfo() const = 0;
    virtual void edit();

    virtual std::string getType() const;
    double      getPrice()  const;
    std::string getTitle()  const;
    std::string getAuthor() const;
    std::string getFormat() const;
    int         getSize()   const;
};

// AudioFile
class AudioFile : public MediaFile {
protected:
    int         duration;
    std::string album;
public:
    AudioFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string al);

    void edit() override;
    void printInfo() const override;

    std::string getType() const override;
    std::string getAlbum() const;
    int getDuration() const;
};

// VideoFile
class VideoFile : public MediaFile {
protected:
    int         duration;
    std::string resolution;
public:
    VideoFile(std::string t, std::string a, double p, int s, std::string f, int d, std::string r);

    void printInfo() const override;
    void edit() override;

    std::string getType() const override;
    std::string getResolution() const;
    int getDuration() const;
};

// ImageFile
class ImageFile : public MediaFile {
protected:
    std::string resolution;
public:
    ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r);

    void printInfo() const override;
    void edit() override;

    std::string getType() const override;
    std::string getResolution() const;
};