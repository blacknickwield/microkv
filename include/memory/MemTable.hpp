#pragma once
#include <memory>
#include <string>

#include "SkipList.hpp"
#include "Allocator.hpp"

namespace microkv {
class MemTable {
public:
    explicit MemTable(std::shared_ptr<Allocator> allocator_);
    MemTable() = delete;
private:
    // std::shared_ptr<SkipList<>> m_table;
    std::shared_ptr<SkipList<std::string, std::string>> table;
    std::shared_ptr<Allocator> allocator;
};

} // namespace microk 

