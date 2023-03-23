#include <cmath>

#include "filter/BloomFilter.hpp"
#include "util/Encoder.hpp"

namespace microkv {

BloomFilter::BloomFilter(uint32_t keysNum, double falsePositive) {
    // The min length of bit set
    uint32_t bitsNum = -1 * static_cast<uint32_t>(keysNum * std::log(falsePositive) / (std::log(2) * std::log(2)));
    uint32_t size = static_cast<uint32_t>((bitsNum + 7) / 8);
    bitSet.resize(size);
    bitsNum = size * 8;
    keyBitsNum = bitsNum / keysNum;

    hashNums = static_cast<uint32_t>(keyBitsNum * std::log(2));
    hashNums = std::max(static_cast<uint32_t>(1), hashNums);
    hashNums = std::min(static_cast<uint32_t>(32), hashNums);
}

BloomFilter::BloomFilter(uint32_t hashNums, const std::string &bitSet) : hashNums(hashNums), bitSet(bitSet) {

}

void BloomFilter::Insert(std::vector<std::string> &keys) {
    uint32_t bitLength = bitSet.length() * 8;
    for (const auto &key : keys) {
        if (key.empty()) continue;
        uint32_t h = encoder::MurmurHash2(key.c_str(), static_cast<uint32_t>(key.length()));
        uint32_t delta = (h >> 17) | (h << 15);

        for (uint32_t i = 0; i < hashNums; ++i) {
            uint32_t position = h % bitLength;
            bitSet[position / 8] |= (1 << (position % 8));
            h += delta;
        }
    }
}

auto BloomFilter::Exist(const std::string &key) const -> bool {
    if (key.empty()) return false;

    uint32_t bitLength = bitSet.length() * 8;
    uint32_t h = encoder::MurmurHash2(key.c_str(), static_cast<uint32_t>(key.length()));
    uint32_t delta = (h >> 17) | (h << 15);

    for (uint32_t i = 0; i < hashNums; ++i) {
        uint32_t position = h % bitLength;
        if (((bitSet[position / 8] >> (position % 8)) & 1) == 0) {
            return false;
        }

        h += delta;
    }

    return true;
}


auto BloomFilter::Data() const -> const std::string& {
    return bitSet;
}


auto BloomFilter::HashNums() const -> uint32_t {
    return hashNums;
}

}