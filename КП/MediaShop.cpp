#include "MediaShop.h"
#include "MediaFile.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <sstream>
#include <vector>
#include <cstdio>  

static bool writeStr(const std::string& s, std::ofstream& out) {
    if (s.size() > static_cast<size_t>(std::numeric_limits<int>::max())) return false;
    int len = static_cast<int>(s.size());
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (!out.good()) return false;
    if (len > 0) {
        out.write(s.data(), len);
        if (!out.good()) return false;
    }
    return true;
}

static bool readStr(std::string& s, std::ifstream& in) {
    int len = 0;
    if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
    if (len < 0) return false;
    s.clear();
    if (len == 0) return true;
    s.resize(static_cast<size_t>(len));
    if (!in.read(&s[0], len)) return false;
    return true;
}

static std::string escapeField(const std::string& s) {
    std::string r;
    r.reserve(s.size());
    for (char c : s) {
        if (c == '\\') { r.push_back('\\'); r.push_back('\\'); }
        else if (c == '|') { r.push_back('\\'); r.push_back('|'); }
        else r.push_back(c);
    }
    return r;
}

static std::vector<std::string> splitEscaped(const std::string& line) {
    std::vector<std::string> out;
    std::string cur;
    cur.reserve(32);
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '\\' && i + 1 < line.size()) {
            cur.push_back(line[i + 1]);
            ++i;
        }
        else if (c == '|') {
            out.push_back(cur);
            cur.clear();
        }
        else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

static std::string makePendingLine(const std::string& seller, const std::shared_ptr<MediaFile>& f) {
    std::ostringstream ss;
    auto esc = [](const std::string& s) { return escapeField(s); };
    ss << esc(seller) << '|' << esc(f->getType()) << '|' << esc(f->getTitle()) << '|' << esc(f->getAuthor())
        << '|' << f->getPrice() << '|' << f->getSize() << '|' << esc(f->getFormat()) << '|';
    std::string type = f->getType();
    if (type == "Audio") {
        ss << f->getDuration() << '|' << esc(f->getAlbum());
    }
    else if (type == "Video") {
        ss << f->getDuration() << '|' << esc(f->getResolution());
    }
    else if (type == "Image") {
        ss << esc(f->getResolution()) << '|';
    }
    else {
        ss << '|';
    }
    return ss.str();
}

static bool savePendingAtomicFile(const std::vector<std::string>& pending, const std::string& fname) {
    std::string tmp = fname + ".tmp";
    std::ofstream out(tmp, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Не удалось открыть временный файл для записи: " << tmp << "\n";
        return false;
    }
    for (const auto& line : pending) {
        out << line << '\n';
        if (out.fail()) {
            std::cerr << "Ошибка записи строки в файл: " << tmp << "\n";
            out.close();
            std::remove(tmp.c_str());
            return false;
        }
    }
    out.close();
    if (!out.good()) {
        std::cerr << "Ошибка при завершении записи файла: " << tmp << "\n";
        std::remove(tmp.c_str());
        return false;
    }
    std::remove(fname.c_str());
    if (std::rename(tmp.c_str(), fname.c_str()) != 0) {
        std::cerr << "Не удалось переименовать '" << tmp << "' в '" << fname << "'\n";
        std::remove(tmp.c_str());
        return false;
    }
    return true;
}

static int getTypeCode(const std::string& t) {
    if (t == "Audio") return 0;
    if (t == "Video") return 1;
    if (t == "Image") return 2;
    return 255;
}
static int getFileTypeCode(const MediaFile* p) {
    if (!p) return 255;
    return getTypeCode(p->getType());
}


void MediaShop::addSeller(const std::shared_ptr<Seller>& s) { sellers.push_back(s); }

std::vector<std::shared_ptr<Seller>>& MediaShop::getSellers() { return sellers; }

const std::vector<std::shared_ptr<Seller>>& MediaShop::getSellers() const { return sellers; }

void MediaShop::showSellers() const {
    std::cout << "Список продавцов:\n";
    for (size_t i = 0; i < sellers.size(); ++i) {
        auto sellerPtr = sellers[i];
        if (sellerPtr) std::cout << (i + 1) << ". " << sellerPtr->getLogin() << '\n';
        else std::cout << (i + 1) << ". (пусто)\n";
    }
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


// Сохраняет состояние магазина в бинарный файл
bool MediaShop::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc); // Открываем файл для бинарной записи с очисткой
    if (!out.is_open()) { // Проверяем успешность открытия файла
        std::cerr << "save: не удалось открыть файл: " << filename << "\n";
        return false;
    }
    int sellerCount = static_cast<int>(sellers.size()); // Получаем количество продавцов
    out.write(reinterpret_cast<const char*>(&sellerCount), sizeof(sellerCount)); // Записываем количество продавцов
    if (!out.good()) { out.close(); return false; } // Проверяем успешность записи
    for (size_t si = 0; si < sellers.size(); si++) { // Перебираем всех продавцов
        auto sellerPtr = sellers[si]; // Получаем текущего продавца
        std::string login;
        if (sellerPtr) login = sellerPtr->getLogin(); // Получаем логин продавца
        if (!writeStr(login, out)) { out.close(); return false; } // Записываем логин строкой
        double balance = 0.0;
        if (sellerPtr) balance = sellerPtr->getBalance(); // Получаем баланс продавца
        out.write(reinterpret_cast<const char*>(&balance), sizeof(balance)); // Записываем баланс
        if (!out.good()) { out.close(); return false; }
        int filesCount = 0;
        if (sellerPtr) filesCount = static_cast<int>(sellerPtr->getFilesCount()); // Получаем количество файлов продавца
        out.write(reinterpret_cast<const char*>(&filesCount), sizeof(filesCount)); // Записываем количество файлов
        if (!out.good()) { out.close(); return false; }
        for (int fi = 1; fi <= filesCount; fi++) { // Перебираем файлы продавца
            auto f = sellerPtr->getFile(fi); // Получаем файл по индексу
            if (!f) { // Если файл nullptr
				int code = 255; // Код для отсутствующего файла
                out.write(reinterpret_cast<const char*>(&code), sizeof(code)); // Записываем код
                writeStr(std::string(), out); // Пустое название
                writeStr(std::string(), out); // Пустой автор
                double p0 = 0.0; out.write(reinterpret_cast<const char*>(&p0), sizeof(p0)); // Нулевая цена
                int s0 = 0; out.write(reinterpret_cast<const char*>(&s0), sizeof(s0)); // Нулевой размер
                writeStr(std::string(), out); // Пустой формат
                continue; // Переходим к следующему файлу
            }
            int code = getFileTypeCode(f.get()); // Получаем код типа файла 0-Audio 1-Video 2-Image
            out.write(reinterpret_cast<const char*>(&code), sizeof(code)); // Записываем код типа
            if (!out.good()) { out.close(); return false; }
            if (!writeStr(f->getTitle(), out)) { out.close(); return false; } // Записываем название
            if (!writeStr(f->getAuthor(), out)) { out.close(); return false; } // Записываем автора
            double price = f->getPrice();
            out.write(reinterpret_cast<const char*>(&price), sizeof(price)); // Записываем цену
            if (!out.good()) { out.close(); return false; }
            int sz = f->getSize();
            out.write(reinterpret_cast<const char*>(&sz), sizeof(sz)); // Записываем размер
            if (!out.good()) { out.close(); return false; }
            if (!writeStr(f->getFormat(), out)) { out.close(); return false; } // Записываем формат
            switch (code) { // В зависимости от типа файла записываем дополнительные поля
            case 0: { // Audio
                int dur = f->getDuration();
                out.write(reinterpret_cast<const char*>(&dur), sizeof(dur)); // Длительность
                if (!out.good()) { out.close(); return false; }
                if (!writeStr(f->getAlbum(), out)) { out.close(); return false; } // Альбом
                break;
            }
            case 1: { // Video
                int dur = f->getDuration();
                out.write(reinterpret_cast<const char*>(&dur), sizeof(dur)); // Длительность
                if (!out.good()) { out.close(); return false; }
                if (!writeStr(f->getResolution(), out)) { out.close(); return false; } // Разрешение
                break;
            }
            case 2: { // Image
                if (!writeStr(f->getResolution(), out)) { out.close(); return false; } // Разрешение
                break;
            }
            default: break; 
            }
        }
    }
    out.close(); // Закрываем файл
    return out.good(); // true если успешно
}

bool MediaShop::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        return false;
    }

    int sellerCount = 0;
    if (!in.read(reinterpret_cast<char*>(&sellerCount), sizeof(sellerCount))) { in.close(); return false; }
    if (sellerCount < 0) { in.close(); return false; }

    sellers.clear();

    for (int si = 0; si < sellerCount; ++si) {
        std::string login;
        if (!readStr(login, in)) { in.close(); return false; }

        double balance = 0.0;
        if (!in.read(reinterpret_cast<char*>(&balance), sizeof(balance))) { in.close(); return false; }

        int filesCount = 0;
        if (!in.read(reinterpret_cast<char*>(&filesCount), sizeof(filesCount))) { in.close(); return false; }
        if (filesCount < 0) { in.close(); return false; }

        std::shared_ptr<Seller> sellerPtr = findSeller(login);
        if (!sellerPtr) {
            sellerPtr = std::make_shared<Seller>(login, balance);
            sellers.push_back(sellerPtr);
        }
        else {
            double diff = balance - sellerPtr->getBalance();
            if (diff > 0.0) sellerPtr->addBalance(diff);
        }

        for (int fi = 0; fi < filesCount; ++fi) {
            int typeCode = 255;
            if (!in.read(reinterpret_cast<char*>(&typeCode), sizeof(typeCode))) { in.close(); return false; }

            std::string title;
            if (!readStr(title, in)) { in.close(); return false; }

            std::string author;
            if (!readStr(author, in)) { in.close(); return false; }

            double price = 0.0;
            if (!in.read(reinterpret_cast<char*>(&price), sizeof(price))) { in.close(); return false; }

            int sz = 0;
            if (!in.read(reinterpret_cast<char*>(&sz), sizeof(sz))) { in.close(); return false; }

            std::string format;
            if (!readStr(format, in)) { in.close(); return false; }

            switch (typeCode) {
            case 0: {
                int dur = 0;
                if (!in.read(reinterpret_cast<char*>(&dur), sizeof(dur))) { in.close(); return false; }
                std::string album;
                if (!readStr(album, in)) { in.close(); return false; }
                sellerPtr->addFile(std::make_shared<AudioFile>(title, author, price, sz, format, dur, album));
                break;
            }
            case 1: {
                int dur = 0;
                if (!in.read(reinterpret_cast<char*>(&dur), sizeof(dur))) { in.close(); return false; }
                std::string res;
                if (!readStr(res, in)) { in.close(); return false; }
                sellerPtr->addFile(std::make_shared<VideoFile>(title, author, price, sz, format, dur, res));
                break;
            }
            case 2: {
                std::string res;
                if (!readStr(res, in)) { in.close(); return false; }
                sellerPtr->addFile(std::make_shared<ImageFile>(title, author, price, sz, format, res));
                break;
            }
            default:
                break;
            }
        }
    }

    in.close();
    return true;
}


bool MediaShop::loadPending(const std::string& filename) {
    std::string fname = filename.empty() ? pendingFilename : filename;
    std::ifstream in(fname);
    pending.clear();
    if (!in.is_open()) return false;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) pending.push_back(line);
    }
    in.close();
    return true;
}

bool MediaShop::savePending(const std::string& filename) const {
    std::string fname = filename.empty() ? pendingFilename : filename;
    return savePendingAtomicFile(pending, fname);
}

// отправить файл продавца на модерацию
bool MediaShop::submitFileForApproval(const std::string& sellerLogin, int fileIdx) {
    auto seller = findSeller(sellerLogin); // Находим продавца по логину
    if (!seller) return false; // Если продавец не найден, возвращаем false
    auto file = seller->getFile(fileIdx); // Получаем файл продавца по индексу
    if (!file) return false; // Если файл не найден, возвращаем false
    std::string line = makePendingLine(sellerLogin, file); // Формируем строку для очереди 
    pending.push_back(line); // Добавляем строку в очередь
    if (!savePending()) { // Пытаемся сохранить очередь в файл
        pending.pop_back(); // Если сохранение не удалось, удаляем строку из очереди
        return false; // Возвращаем false
    }
    if (!seller->removeFile(fileIdx)) { // Удаляем файл у продавца
        if (!pending.empty()) pending.pop_back(); // Если удаление не удалось, удаляем из очереди
        savePending(); // Пытаемся сохранить изменения в файле очереди
        return false; // Возвращаем false
    }
    return true;
}

// вывести очередь на экран
void MediaShop::listPending() const {
    if (pending.empty()) { std::cout << "Очередь пуста\n"; return; }
    for (size_t i = 0; i < pending.size(); ++i) {
        auto fields = splitEscaped(pending[i]);
        std::string seller = (fields.size() > 0) ? fields[0] : "";
        std::string type = (fields.size() > 1) ? fields[1] : "";
        std::string title = (fields.size() > 2) ? fields[2] : "";
        std::cout << (i + 1) << ". " << seller << " | " << type << " | " << title << '\n';
    }
}

// одобрить первый элемент очереди
bool MediaShop::approveNext() {
    if (pending.empty()) return false; // Если очередь пуста возвращаем false
    std::string line = pending.front(); // Получаем первую строку из очереди
    auto fields = splitEscaped(line); // Разбиваем строку на поля 
    if (fields.size() < 7) { // Проверяем количество полей
        pending.erase(pending.begin()); // Удаляем некорректную запись из очереди
        savePending(); // Сохраняем изменения в файле очереди
        return false; // Возвращаем false
    }
    std::string sellerLogin = fields[0]; // Логин продавца
    std::string type = fields[1]; // Тип файла
    std::string title = fields[2]; // Название файла
    std::string author = fields[3]; // Автор файла
    double price = 0.0; // Цена файла
    int sz = 0; // Размер файла
    try { price = std::stod(fields[4]); }
    catch (const std::invalid_argument&) { price = 0.0; } // Если не число
    try { sz = std::stoi(fields[5]);}
    catch (const std::invalid_argument&) { sz = 0; }
    std::string format = fields[6]; // Формат файла
    std::shared_ptr<MediaFile> newFile; // Указатель на создаваемый файл
    if (type == "Audio") { // Обработка аудиофайла
        int dur = 0; std::string album;
        if (fields.size() > 7) { try { dur = std::stoi(fields[7]); } catch (const std::invalid_argument&) { dur = 0; } } // Длительность
        if (fields.size() > 8) album = fields[8]; // Альбом
        newFile = std::make_shared<AudioFile>(title, author, price, sz, format, dur, album); // Создаём аудиофайл
    }
    else if (type == "Video") { // Обработка видеофайла
        int dur = 0; std::string res;
        if (fields.size() > 7) { try { dur = std::stoi(fields[7]); } catch (const std::invalid_argument&) { dur = 0; } } // Длительность
        if (fields.size() > 8) res = fields[8]; // Разрешение
        newFile = std::make_shared<VideoFile>(title, author, price, sz, format, dur, res); // Создаём видеофайл
    }
    else if (type == "Image") { // Обработка изображения
        std::string res;
        if (fields.size() > 7) res = fields[7]; // Разрешение
        newFile = std::make_shared<ImageFile>(title, author, price, sz, format, res); // Создаём изображение
    }
    else {
        pending.erase(pending.begin()); // Удаляем запись из очереди
        savePending(); // Сохраняем изменения
        return false; // Возвращаем false
    }
    auto seller = findSeller(sellerLogin); // Ищем продавца по логину
    if (!seller) { // Если продавец не найден
        seller = std::make_shared<Seller>(sellerLogin, 0.0); // Создаём нового продавца
        sellers.push_back(seller); // Добавляем в список продавцов
    }
    if (newFile) seller->addFile(newFile); // Если файл создан успешно, добавляем его продавцу
    pending.erase(pending.begin()); // Удаляем запись из очереди
    savePending(); // Сохраняем изменения в файле очереди
    return true;
}

// отклонить первый элемент очереди
bool MediaShop::rejectNext() {
    if (pending.empty()) return false;
    pending.erase(pending.begin());
    savePending();
    return true;
}