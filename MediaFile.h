#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

// User-defined namespace for MediaShop
namespace MediaShopNS {

// Template class for sorting helper
template<typename T>
class SortHelper {
public:
    static void sortByPrice(std::vector<std::shared_ptr<T>>& items, bool ascending = true) {
        std::sort(items.begin(), items.end(), [ascending](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return ascending ? a->getPrice() < b->getPrice() : a->getPrice() > b->getPrice();
        });
    }
    
    static void sortByTitle(std::vector<std::shared_ptr<T>>& items, bool ascending = true) {
        std::sort(items.begin(), items.end(), [ascending](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return ascending ? a->getTitle() < b->getTitle() : a->getTitle() > b->getTitle();
        });
    }
    
    static void sortBySize(std::vector<std::shared_ptr<T>>& items, bool ascending = true) {
        std::sort(items.begin(), items.end(), [ascending](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return ascending ? a->getSize() < b->getSize() : a->getSize() > b->getSize();
        });
    }
};

// Forward declaration for friend functions
class MediaFile;
std::ostream& operator<<(std::ostream& os, const MediaFile& file);
std::istream& operator>>(std::istream& is, MediaFile& file);

// Base class for media files (abstract class)
class MediaFile {
protected:
    std::string title;
    std::string author;
    std::string format;
    double      price;
    int         size;
    
    static int totalFilesCount;  // Static field to track total files
    static double totalSalesAmount;  // Static field to track total sales
    
public:
    MediaFile(std::string t, std::string a, double p, int s, std::string f);
    virtual ~MediaFile();

    // Pure virtual function - makes this an abstract class
    virtual void printInfo() const = 0;
    virtual void edit();

    virtual std::string getType() const;
    double      getPrice()  const;
    std::string getTitle()  const;
    std::string getAuthor() const;
    std::string getFormat() const;
    int         getSize()   const;
    
    // Static methods
    static int getTotalFilesCount();
    static double getTotalSalesAmount();
    static void addSale(double amount);
    static void resetStatistics();
    
    // Friend functions for operator overloading
    friend std::ostream& operator<<(std::ostream& os, const MediaFile& file);
    friend std::istream& operator>>(std::istream& is, MediaFile& file);
    
    // Virtual method for serialization
    virtual std::string serialize() const;
};

// AudioFile class
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
    
    std::string serialize() const override;
    
    // Overloaded operator for comparison
    bool operator==(const AudioFile& other) const;
    bool operator<(const AudioFile& other) const;
};

// VideoFile class
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
    
    std::string serialize() const override;
    
    bool operator==(const VideoFile& other) const;
    bool operator<(const VideoFile& other) const;
};

// ImageFile class
class ImageFile : public MediaFile {
protected:
    std::string resolution;
public:
    ImageFile(std::string t, std::string a, double p, int s, std::string f, std::string r);

    void printInfo() const override;
    void edit() override;

    std::string getType() const override;
    std::string getResolution() const;
    
    std::string serialize() const override;
    
    bool operator==(const ImageFile& other) const;
    bool operator<(const ImageFile& other) const;
};

// Template function for filtering files
template<typename T, typename Predicate>
std::vector<std::shared_ptr<T>> filterFiles(const std::vector<std::shared_ptr<T>>& files, Predicate pred) {
    std::vector<std::shared_ptr<T>> result;
    for (const auto& file : files) {
        if (file && pred(*file)) {
            result.push_back(file);
        }
    }
    return result;
}

} // namespace MediaShopNS

// Bring common types into global scope for compatibility
using MediaShopNS::MediaFile;
using MediaShopNS::AudioFile;
using MediaShopNS::VideoFile;
using MediaShopNS::ImageFile;
using MediaShopNS::SortHelper;
using MediaShopNS::filterFiles;