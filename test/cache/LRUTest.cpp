#include <memory>
#include <string>

#include "cache/LRU.hpp"
#include <gtest/gtest.h>

TEST(LRUTest, SimpleTest) {
    auto lru_cache = std::make_shared<microkv::LRU<std::string, std::string>>(3);

    const std::string hello = "hello"; std::string hello_value = "1";
    const std::string how = "how"; std::string how_value = "2";
    const std::string are = "are"; std::string are_value = "3";
    const std::string you = "you"; std::string you_value = "4";

    std::string *value;

    lru_cache->Insert(hello, &hello_value);
    lru_cache->Insert(how, &how_value);
    lru_cache->Insert(are, &are_value);

    EXPECT_EQ(true, lru_cache->Get(how, value));
    EXPECT_EQ(true, lru_cache->Get(are, value));
    EXPECT_EQ(true, lru_cache->Get(hello, value));

    lru_cache->Insert(you, &you_value);
    EXPECT_EQ(false, lru_cache->Get(how, value));
    EXPECT_EQ(true, lru_cache->Get(are, value));
    EXPECT_EQ(true, lru_cache->Get(hello, value));
    EXPECT_EQ(true, lru_cache->Get(you, value));

    EXPECT_EQ(true, lru_cache->Delete(you));
    EXPECT_EQ(false, lru_cache->Get(you, value));

    const std::string fine = "fine";
    EXPECT_EQ(false, lru_cache->Delete(fine));
}
