#include "MediaShop.h"
#include <iostream>
#include <fstream>
#include <iomanip>

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

bool MediaShop::saveToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::trunc);
    if (!out) return false;

    out << sellers.size() << '\n';
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
            else {
            }
        }
    }

    out.close();
    return true;
}

bool MediaShop::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    sellers.clear();
    std::string line;
    if (!std::getline(in, line)) return false;
    size_t sellersCount = 0;
    try { sellersCount = static_cast<size_t>(std::stoul(line)); }
    catch (...) { return false; }

    for (size_t i = 0; i < sellersCount; ++i) {
        std::string login;
        if (!std::getline(in, login)) break;

        std::string balStr;
        if (!std::getline(in, balStr)) break;
        double balance = 0.0;
        try { balance = std::stod(balStr); }
        catch (...) { balance = 0.0; }

        auto seller = std::make_shared<Seller>(login, balance);

        std::string numFilesStr;
        if (!std::getline(in, numFilesStr)) break;
        int numFiles = 0;
        try { numFiles = std::stoi(numFilesStr); }
        catch (...) { numFiles = 0; }

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
            catch (...) { price = 0.0; }
            try { sz = std::stoi(sizeStr); }
            catch (...) { sz = 0; }

            if (type == "Audio") {
                std::string durStr, album;
                if (!std::getline(in, durStr)) break;
                if (!std::getline(in, album)) break;
                int dur = 0;
                try { dur = std::stoi(durStr); }
                catch (...) { dur = 0; }
                seller->addFile(std::make_shared<AudioFile>(title, author, price, sz, format, dur, album));
            }
            else if (type == "Video") {
                std::string durStr, res;
                if (!std::getline(in, durStr)) break;
                if (!std::getline(in, res)) break;
                int dur = 0;
                try { dur = std::stoi(durStr); }
                catch (...) { dur = 0; }
                seller->addFile(std::make_shared<VideoFile>(title, author, price, sz, format, dur, res));
            }
            else if (type == "Image") {
                std::string res;
                if (!std::getline(in, res)) break;
                seller->addFile(std::make_shared<ImageFile>(title, author, price, sz, format, res));
            }
            else {

            }
        }

        sellers.push_back(seller);
    }

    in.close();
    return true;
}