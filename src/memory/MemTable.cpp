#include "memory/MemTable.hpp"
// #include "memory/MemTableIterator.hpp"

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

auto MemTable::NewIter() const -> MemTableIterator* {
    return new MemTableIterator(table.get());
}

void MemTable::Persist(std::shared_ptr<SSTableBuilder> sstableBuilder) {
    auto *it = NewIter();
    it->Reset();

    while (it->HasNext()) {
        it->Next();
        sstableBuilder->Add(it->Key(), it->Value());
    }

    sstableBuilder->CreateSSTable();
}

}