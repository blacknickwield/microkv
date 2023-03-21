#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace microkv {

class BloomFilter {
public:
    explicit BloomFilter(uint32_t keysNum, double falsePositive);
public:
    void Insert(std::vector<std::string> &keys);
    auto Exist(const std::string &key) const -> bool;
private:
    uint32_t hashNums; // The number of hash function
    // std::vector<uint8_t> bitSet;
    std::string bitSet;
    uint32_t keyBitsNum;
};

}
