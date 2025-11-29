#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"

namespace MediaShopNS {

class MediaShop {
    std::vector<std::shared_ptr<Seller>> sellers;
    
    static int shopInstanceCount;  // Static field
    
public:
    MediaShop();
    ~MediaShop();
    
    void addSeller(const std::shared_ptr<Seller>& s);
    std::vector<std::shared_ptr<Seller>>& getSellers();
    const std::vector<std::shared_ptr<Seller>>& getSellers() const;

    void showSellers() const;
    std::shared_ptr<Seller> getSeller(int idx) const;
    std::shared_ptr<Seller> findSeller(const std::string& login) const;

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    // Search across all sellers
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
        searchAllFiles(const std::string& query) const;
    
    // Filter all files by type
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
        filterAllByType(const std::string& type) const;
    
    // Filter all files by price range
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
        filterAllByPriceRange(double minPrice, double maxPrice) const;
    
    // Get all files sorted by price
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
        getAllFilesSortedByPrice(bool ascending = true) const;
    
    // Get all files sorted by title
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
        getAllFilesSortedByTitle(bool ascending = true) const;
    
    // Static method
    static int getShopInstanceCount();
    
    // Generate full catalog report
    std::string generateCatalogReport() const;
};

} // namespace MediaShopNS

using MediaShopNS::MediaShop;