#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../filter/BloomFilter.hpp"
#include "../util/Status.hpp"

namespace microkv {

class FilterBlockBuilder {
public:
    FilterBlockBuilder(uint32_t keysNum, double false_positive);
    void Add(const std::string &key);
    auto CreateFilterBlock() -> Status;
    auto Data() const -> const std::string&;
    auto Size() const -> uint32_t;
private:
    void InsertFilter();
private:
    std::shared_ptr<BloomFilter> filter;
    std::string data;
    std::vector<std::string> keys;
};

}