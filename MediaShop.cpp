#include "MediaShop.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <stdexcept>

namespace MediaShopNS {

// Static member initialization
int MediaShop::shopInstanceCount = 0;

MediaShop::MediaShop() {
    ++shopInstanceCount;
}

MediaShop::~MediaShop() {
    --shopInstanceCount;
}

void MediaShop::addSeller(const std::shared_ptr<Seller>& s) { sellers.push_back(s); }
std::vector<std::shared_ptr<Seller>>& MediaShop::getSellers() { return sellers; }
const std::vector<std::shared_ptr<Seller>>& MediaShop::getSellers() const { return sellers; }

void MediaShop::showSellers() const {
    std::cout << "Список продавцов:\n";
    for (size_t i = 0; i < sellers.size(); ++i)
        std::cout << i + 1 << ". " << sellers[i]->getLogin() << std::endl;
}

std::shared_ptr<Seller> MediaShop::getSeller(int idx) const {
    if (idx < 1 || idx > static_cast<int>(sellers.size())) return nullptr;
    return sellers[idx - 1];
}

std::shared_ptr<Seller> MediaShop::findSeller(const std::string& login) const {
    for (const auto& s : sellers) {
        if (s && s->getLogin() == login) return s;
    }
    return nullptr;
}

int MediaShop::getShopInstanceCount() { return shopInstanceCount; }

std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
MediaShop::searchAllFiles(const std::string& query) const {
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> results;
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);
    
    for (const auto& seller : sellers) {
        if (!seller) continue;
        for (const auto& file : seller->getFiles()) {
            if (!file) continue;
            std::string titleLower = file->getTitle();
            std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            std::string authorLower = file->getAuthor();
            std::transform(authorLower.begin(), authorLower.end(), authorLower.begin(), ::tolower);
            
            if (titleLower.find(queryLower) != std::string::npos ||
                authorLower.find(queryLower) != std::string::npos) {
                results.push_back({seller, file});
            }
        }
    }
    return results;
}

std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
MediaShop::filterAllByType(const std::string& type) const {
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> results;
    for (const auto& seller : sellers) {
        if (!seller) continue;
        for (const auto& file : seller->getFiles()) {
            if (file && file->getType() == type) {
                results.push_back({seller, file});
            }
        }
    }
    return results;
}

std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
MediaShop::filterAllByPriceRange(double minPrice, double maxPrice) const {
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> results;
    for (const auto& seller : sellers) {
        if (!seller) continue;
        for (const auto& file : seller->getFiles()) {
            if (file && file->getPrice() >= minPrice && file->getPrice() <= maxPrice) {
                results.push_back({seller, file});
            }
        }
    }
    return results;
}

std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
MediaShop::getAllFilesSortedByPrice(bool ascending) const {
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> allFiles;
    for (const auto& seller : sellers) {
        if (!seller) continue;
        for (const auto& file : seller->getFiles()) {
            if (file) {
                allFiles.push_back({seller, file});
            }
        }
    }
    
    std::sort(allFiles.begin(), allFiles.end(), 
        [ascending](const auto& a, const auto& b) {
            return ascending ? a.second->getPrice() < b.second->getPrice() 
                            : a.second->getPrice() > b.second->getPrice();
        });
    
    return allFiles;
}

std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> 
MediaShop::getAllFilesSortedByTitle(bool ascending) const {
    std::vector<std::pair<std::shared_ptr<Seller>, std::shared_ptr<MediaFile>>> allFiles;
    for (const auto& seller : sellers) {
        if (!seller) continue;
        for (const auto& file : seller->getFiles()) {
            if (file) {
                allFiles.push_back({seller, file});
            }
        }
    }
    
    std::sort(allFiles.begin(), allFiles.end(), 
        [ascending](const auto& a, const auto& b) {
            return ascending ? a.second->getTitle() < b.second->getTitle() 
                            : a.second->getTitle() > b.second->getTitle();
        });
    
    return allFiles;
}

std::string MediaShop::generateCatalogReport() const {
    std::ostringstream report;
    
    std::time_t now = std::time(nullptr);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    report << "========================================\n";
    report << "         КАТАЛОГ МЕДИАФАЙЛОВ           \n";
    report << "========================================\n";
    report << "Дата отчёта: " << timeStr << "\n";
    report << "----------------------------------------\n";
    
    int totalFiles = 0;
    double totalValue = 0.0;
    
    for (size_t i = 0; i < sellers.size(); ++i) {
        const auto& seller = sellers[i];
        if (!seller) continue;
        
        report << "\nПродавец: " << seller->getLogin() << "\n";
        report << "Баланс: " << std::fixed << std::setprecision(2) << seller->getBalance() << " руб.\n";
        
        const auto& files = seller->getFiles();
        if (files.empty()) {
            report << "  (нет файлов)\n";
        } else {
            for (size_t j = 0; j < files.size(); ++j) {
                const auto& file = files[j];
                if (file) {
                    report << "  " << (j + 1) << ". [" << file->getType() << "] " 
                           << file->getTitle() << "\n";
                    report << "     Автор: " << file->getAuthor() << "\n";
                    report << "     Цена: " << std::fixed << std::setprecision(2) 
                           << file->getPrice() << " руб.\n";
                    report << "     Размер: " << file->getSize() << " МБ\n";
                    totalFiles++;
                    totalValue += file->getPrice();
                }
            }
        }
    }
    
    report << "\n----------------------------------------\n";
    report << "ИТОГО продавцов: " << sellers.size() << "\n";
    report << "ИТОГО файлов: " << totalFiles << "\n";
    report << "Общая стоимость каталога: " << std::fixed << std::setprecision(2) 
           << totalValue << " руб.\n";
    report << "========================================\n";
    
    return report.str();
}

bool MediaShop::saveToFile(const std::string& filename) const {
    try {
        std::ofstream out(filename, std::ios::trunc);
        if (!out) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }

        out << sellers.size() << '\n';
        if (!out) {
            throw std::runtime_error("Ошибка записи в файл");
        }
        
        for (const auto& s : sellers) {
            if (!s) continue;
            out << s->getLogin() << '\n';
            out << std::fixed << std::setprecision(2) << s->getBalance() << '\n';

            size_t cnt = s->getFilesCount();
            out << cnt << '\n';
            for (size_t i = 1; i <= cnt; ++i) {
                auto f = s->getFile(static_cast<int>(i));
                if (!f) continue;
                std::string type = f->getType();
                out << type << '\n';
                out << f->getTitle() << '\n';
                out << f->getAuthor() << '\n';
                out << std::fixed << std::setprecision(2) << f->getPrice() << '\n';
                out << f->getSize() << '\n';
                out << f->getFormat() << '\n';
                if (type == "Audio") {
                    auto af = std::dynamic_pointer_cast<AudioFile>(f);
                    if (af) {
                        out << af->getDuration() << '\n';
                        out << af->getAlbum() << '\n';
                    }
                    else {
                        out << 0 << '\n' << '\n';
                    }
                }
                else if (type == "Video") {
                    auto vf = std::dynamic_pointer_cast<VideoFile>(f);
                    if (vf) {
                        out << vf->getDuration() << '\n';
                        out << vf->getResolution() << '\n';
                    }
                    else {
                        out << 0 << '\n' << '\n';
                    }
                }
                else if (type == "Image") {
                    auto im = std::dynamic_pointer_cast<ImageFile>(f);
                    if (im) {
                        out << im->getResolution() << '\n';
                    }
                    else {
                        out << '\n';
                    }
                }
            }
            
            if (!out) {
                throw std::runtime_error("Ошибка записи данных продавца");
            }
        }

        out.close();
        if (out.fail()) {
            throw std::runtime_error("Ошибка закрытия файла");
        }
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool MediaShop::loadFromFile(const std::string& filename) {
    try {
        std::ifstream in(filename);
        if (!in) {
            // File doesn't exist - not an error, just no data to load
            return false;
        }

        sellers.clear();
        std::string line;
        if (!std::getline(in, line)) return false;
        
        size_t sellersCount = 0;
        try { 
            sellersCount = static_cast<size_t>(std::stoul(line)); 
        }
        catch (const std::exception&) { 
            throw std::runtime_error("Некорректный формат файла: количество продавцов");
        }

        for (size_t i = 0; i < sellersCount; ++i) {
            std::string login;
            if (!std::getline(in, login)) break;

            std::string balStr;
            if (!std::getline(in, balStr)) break;
            double balance = 0.0;
            try { balance = std::stod(balStr); }
            catch (const std::exception&) { balance = 0.0; }

            auto seller = std::make_shared<Seller>(login, balance);

            std::string numFilesStr;
            if (!std::getline(in, numFilesStr)) break;
            int numFiles = 0;
            try { numFiles = std::stoi(numFilesStr); }
            catch (const std::exception&) { numFiles = 0; }

            for (int f = 0; f < numFiles; ++f) {
                std::string type;
                if (!std::getline(in, type)) break;
                std::string title, author, priceStr, sizeStr, format;
                if (!std::getline(in, title)) break;
                if (!std::getline(in, author)) break;
                if (!std::getline(in, priceStr)) break;
                if (!std::getline(in, sizeStr)) break;
                if (!std::getline(in, format)) break;
                
                double price = 0.0;
                int sz = 0;
                try { price = std::stod(priceStr); }
                catch (const std::exception&) { price = 0.0; }
                try { sz = std::stoi(sizeStr); }
                catch (const std::exception&) { sz = 0; }

                if (type == "Audio") {
                    std::string durStr, album;
                    if (!std::getline(in, durStr)) break;
                    if (!std::getline(in, album)) break;
                    int dur = 0;
                    try { dur = std::stoi(durStr); }
                    catch (const std::exception&) { dur = 0; }
                    seller->addFile(std::make_shared<AudioFile>(title, author, price, sz, format, dur, album));
                }
                else if (type == "Video") {
                    std::string durStr, res;
                    if (!std::getline(in, durStr)) break;
                    if (!std::getline(in, res)) break;
                    int dur = 0;
                    try { dur = std::stoi(durStr); }
                    catch (const std::exception&) { dur = 0; }
                    seller->addFile(std::make_shared<VideoFile>(title, author, price, sz, format, dur, res));
                }
                else if (type == "Image") {
                    std::string res;
                    if (!std::getline(in, res)) break;
                    seller->addFile(std::make_shared<ImageFile>(title, author, price, sz, format, res));
                }
            }

            sellers.push_back(seller);
        }

        in.close();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    }
}

} // namespace MediaShopNS