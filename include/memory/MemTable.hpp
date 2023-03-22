#pragma once
#include <memory>
#include <string>

#include "SkipList.hpp"
#include "Allocator.hpp"
#include "MemTableIterator.hpp"

#include "../sstable/SSTableBuilder.hpp"

namespace microkv {
class MemTableIterator;

class MemTable {
public:
    explicit MemTable(std::shared_ptr<Allocator> allocator_);
    MemTable() = delete;
public:
    auto Add(const std::string &key, const std::string &value) -> bool;
    auto Get(const std::string &key, std::string &value) const -> bool;
    auto Update(const std::string &key, const std::string &value) -> bool;
    auto Delete(const std::string &key) -> bool;
    auto NewIter() const -> MemTableIterator*;
    void Persist(std::shared_ptr<SSTableBuilder> sstableBuilder);
private:
    std::shared_ptr<SkipList<std::string>> table;
    std::shared_ptr<Allocator> allocator;
};

} // namespace microk 

