#include <vector>
#include <string>
#include <memory>
#include <gtest/gtest.h>

#include "filter/BloomFilter.hpp"

TEST(BloomFilterTest, SimpleTest) {
    std::vector<std::string> data;
    uint32_t keysNum = 10000;
    double false_positive = 0.01;
    for (size_t i = 0; i < keysNum; ++i) {
        data.push_back("key" + std::to_string(i));
    }

    auto bloomFilter = std::make_shared<microkv::BloomFilter>(keysNum, false_positive);
    bloomFilter->Insert(data);
    uint32_t correct = 0;
    for (uint32_t i = 0; i < keysNum; ++i) {
        const std::string key = "key" + std::to_string(i);
        if (bloomFilter->Exist(key)) {
            ++correct;
        }
    }
    EXPECT_EQ(correct, keysNum);

    uint32_t fake = 0;
    for (uint32_t i = keysNum; i < 2 * keysNum; ++i) {
        const std::string key = "key" + std::to_string(i);
        if (bloomFilter->Exist(key)) {
            ++fake;
        }
    }
    EXPECT_LT(fake, 1500);
}