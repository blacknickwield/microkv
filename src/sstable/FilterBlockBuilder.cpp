#include "sstable/FilterBlockBuilder.hpp"
#include "util/Encoder.hpp"

namespace microkv {

FilterBlockBuilder::FilterBlockBuilder(uint32_t keysNum, double false_positive) {
    filter = std::make_shared<BloomFilter>(keysNum, false_positive);
}

void FilterBlockBuilder::Add(const std::string &key) {
    keys.push_back(key);
}

auto FilterBlockBuilder::CreateFilterBlock() -> Status {
    if (keys.empty()) {
        return {};
    }
    InsertFilter();
    return {};
}


void FilterBlockBuilder::InsertFilter() {
    filter->Insert(keys);
    data = filter->Data();
    encoder::Put32Bit(data, filter->HashNums());
}

auto FilterBlockBuilder::Data() const -> const std::string& {
    return data;
}

auto FilterBlockBuilder::Size() const -> uint32_t {
    return static_cast<uint32_t>(data.length());
}
}