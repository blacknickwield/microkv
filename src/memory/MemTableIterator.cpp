#include "memory/MemTableIterator.hpp"

namespace microkv {

MemTableIterator::MemTableIterator(SkipList<std::string> *skipList) {
    iter = std::make_shared<SLIter>(skipList);
}

auto MemTableIterator::HasNext() const -> bool {
    return iter->HasNext();
}


void MemTableIterator::Next() {
    iter->Next();
}

void MemTableIterator::Reset() {
    iter->Reset();
}

auto MemTableIterator::Key() const -> const std::string& {
    return iter->Key();
}

auto MemTableIterator::Value() const -> std::string {
    return iter->Value();
}

} // namespace microk 
