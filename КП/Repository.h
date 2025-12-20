#include <set>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

template<typename T, typename Comparator = std::less<std::shared_ptr<T>>>
class Repository {
private:
    std::set<std::shared_ptr<T>, Comparator> items;
    std::string filename;

public:
    Repository(const std::string& file = "") : filename(file) {}

    void add(std::shared_ptr<T> item) {
        items.insert(item);
    }

    bool remove(std::shared_ptr<T> item) {
        return items.erase(item) > 0;
    }

    std::shared_ptr<T> find(const std::string& id) const {
        auto it = std::find_if(items.begin(), items.end(),
            [&](const auto& item) { return item->getID() == id; });
        return it != items.end() ? *it : nullptr;
    }

    template<typename Predicate>
    std::vector<std::shared_ptr<T>> filter(Predicate pred) const {
        std::vector<std::shared_ptr<T>> result;
        std::copy_if(items.begin(), items.end(), std::back_inserter(result), pred);
        return result;
    }

    template<typename Function>
    void forEach(Function func) const {
        for (const auto& item : items) {
            func(item);
        }
    }

    std::vector<std::shared_ptr<T>> getAll() const {
        return std::vector<std::shared_ptr<T>>(items.begin(), items.end());
    }

    size_t size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    void clear() { items.clear(); }
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }
};