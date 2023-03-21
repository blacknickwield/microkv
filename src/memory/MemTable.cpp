#include "memory/MemTable.hpp"

namespace microkv {

MemTable::MemTable(std::shared_ptr<Allocator> allocator_) : allocator(allocator_) {
    table = std::make_shared<SkipList<std::string>>(allocator);

}

auto MemTable::Add(const std::string &key, const std::string &value) -> bool {
    return table->Insert(key, value);
}



auto MemTable::Get(const std::string &key, std::string &value) const -> bool {
    return table->Find(key, value);
}

auto MemTable::Update(const std::string &key, const std::string &value) -> bool {
    table->Erase(key);
    return table->Insert(key, value);
}


auto MemTable::Delete(const std::string &key) -> bool {
    if (table->Count(key)) {
        return table->Erase(key);
    }
    return false;
}

}