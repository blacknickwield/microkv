#include <iostream>
#include <gtest/gtest.h>

#include "memory/MemTable.hpp"

TEST(MemTableTest, SimpleTest) {
    auto allocator = std::make_shared<microkv::Allocator>();
    auto memtable = std::make_shared<microkv::MemTable>(allocator);

    for (int i = 0; i < 100; ++i) {
        const std::string key = "key" + std::to_string(i);
        const std::string value = "value" + std::to_string(i);
        memtable->Add(key, value);
    }

    std::string v;
    for (int i = 0; i < 100; ++i) {
        const std::string key = "key" + std::to_string(i);
        const std::string value = "value" + std::to_string(i);
        EXPECT_EQ(memtable->Get(key, v), true);
        EXPECT_EQ(v, value);
    }

    const std::string key1 = "key1";
    const std::string value1 = "value1";
    EXPECT_EQ(false, memtable->Add(key1, value1));
}