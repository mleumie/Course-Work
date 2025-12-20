#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"

class MediaShop {
    std::vector<std::shared_ptr<Seller>> sellers;

    std::vector<std::string> pending;
    std::string pendingFilename = "pending.txt";

public:
    void addSeller(const std::shared_ptr<Seller>& s);
    std::vector<std::shared_ptr<Seller>>& getSellers();
    const std::vector<std::shared_ptr<Seller>>& getSellers() const;

    void showSellers() const;
    std::shared_ptr<Seller> getSeller(int idx) const;
    std::shared_ptr<Seller> findSeller(const std::string& login) const;

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    bool submitFileForApproval(const std::string& sellerLogin, int fileIdx);

    // Загрузить/сохранить очередь
    bool loadPending(const std::string& filename = "");
    bool savePending(const std::string& filename = "") const;

    // Посмотреть очередь 
    void listPending() const;

    // Администратор: одобрить/отклонить первую заявку
    bool approveNext(); 
    bool rejectNext();  
};