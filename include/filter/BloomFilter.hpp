#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace microkv {

class BloomFilter {
public:
    explicit BloomFilter(uint32_t keysNum, double falsePositive);
    explicit BloomFilter(uint32_t hashNums, const std::string &bitSet);
public:
    void Insert(std::vector<std::string> &keys);
    auto Exist(const std::string &key) const -> bool;
    auto Data() const -> const std::string&;
    auto HashNums() const -> uint32_t;
private:
    uint32_t hashNums; // The number of hash function
    // std::vector<uint8_t> bitSet;
    std::string bitSet;
    uint32_t keyBitsNum;
};

}
