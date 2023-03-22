#pragma once

#include <memory>

#include "SkipList.hpp"

namespace microkv {
class MemTableIterator {
public:
    explicit MemTableIterator(SkipList<std::string> *skipList);
    auto HasNext() const -> bool;
    void Next();
    void Reset();
    auto Key() const -> const std::string&;
    // auto Value() const -> const std::string&;
    auto Value() const -> std::string;
private:
    using SLIter = SkipList<std::string>::SkipListIterator;
    std::shared_ptr<SLIter> iter;
};

}