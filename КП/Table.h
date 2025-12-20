#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

namespace Table {

    inline std::string trunc(const std::string& s, std::size_t w) {
        if (s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    }

    static const int W_IDX = 4;
    static const int W_TYPE = 8;
    static const int W_TITLE = 30;
    static const int W_AUTHOR = 16;
    static const int W_FMT = 8;
    static const int W_PRICE = 10;
    static const int W_SIZE = 8;
    static const int W_SELLER = 16;

    template<typename RowT>
    void printFilesTable(const std::vector<RowT>& rows) {
        std::cout << std::left
            << std::setw(W_IDX) << "#" << ' '
            << std::setw(W_TYPE) << "Type" << ' '
            << std::setw(W_TITLE) << "Title" << ' '
            << std::setw(W_AUTHOR) << "Author" << ' '
            << std::setw(W_FMT) << "Fmt" << ' '
            << std::right
            << std::setw(W_PRICE) << "Price" << ' '
            << std::setw(W_SIZE) << "Size" << ' '
            << std::left
            << std::setw(W_SELLER) << "Seller"
            << std::endl;

        int total = W_IDX + 1 + W_TYPE + 1 + W_TITLE + 1 + W_AUTHOR + 1 + W_FMT + 1 + W_PRICE + 1 + W_SIZE + 1 + W_SELLER;
        if (total < 0) total = 0;
        std::cout << std::string(total, '-') << std::endl;

        for (std::size_t i = 0; i < rows.size(); ++i) {
            const RowT& r = rows[i];
            std::string type = r.file ? r.file->getType() : std::string();
            std::string title = r.file ? r.file->getTitle() : std::string();
            std::string author = r.file ? r.file->getAuthor() : std::string();
            std::string fmt = r.file ? r.file->getFormat() : std::string();
            double price = r.file ? r.file->getPrice() : 0.0;
            int size = r.file ? r.file->getSize() : 0;
            std::string seller = r.seller ? r.seller->getLogin() : std::string();

            std::cout << std::left
                << std::setw(W_IDX) << static_cast<int>(i + 1) << ' '
                << std::setw(W_TYPE) << trunc(type, W_TYPE) << ' '
                << std::setw(W_TITLE) << trunc(title, W_TITLE) << ' '
                << std::setw(W_AUTHOR) << trunc(author, W_AUTHOR) << ' '
                << std::setw(W_FMT) << trunc(fmt, W_FMT) << ' '
                << std::right << std::fixed << std::setprecision(2)
                << std::setw(W_PRICE) << price << ' '
                << std::setw(W_SIZE) << size << ' '
                << std::left
                << std::setw(W_SELLER) << trunc(seller, W_SELLER)
                << std::endl;
        }

        // avoid Windows macro collision with std::max — use conditional
        std::cout << std::string((total > 0) ? total : 0, '-') << std::endl;
    }
}

#endif // TABLE_H