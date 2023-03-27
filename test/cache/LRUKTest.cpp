#include <memory>
#include <string>

#include "cache/LRUK.hpp"
#include <gtest/gtest.h>

TEST(LRUKTest, SimpleTest) {
    auto lruk_cache = std::make_shared<microkv::LRUK<std::string, std::string>>(7, 2);
    const std::string one = "1"; std::string one_value = "1";
    const std::string two = "2"; std::string two_value = "2";
    const std::string three = "3"; std::string three_value = "3";
    const std::string four = "4"; std::string four_value = "4";
    const std::string five = "5"; std::string five_value = "5";
    const std::string six = "6"; std::string six_value = "6";
    const std::string seven = "7"; std::string seven_value = "7";

    lruk_cache->Insert(one, &one_value);
    lruk_cache->Insert(two, &two_value);
    lruk_cache->Insert(three, &three_value);
    lruk_cache->Insert(four, &four_value);
    lruk_cache->Insert(five, &five_value);
    lruk_cache->Insert(six, &six_value);
    lruk_cache->Insert(seven, &seven_value);
    
    std::string *value;
    EXPECT_EQ(lruk_cache->Get(one, value), true);
    EXPECT_EQ(*value, one);

    const std::string eight = "8"; std::string eight_value = "8";
    lruk_cache->Insert(eight, &eight_value);
    EXPECT_EQ(lruk_cache->Get(two, value), false);

    EXPECT_EQ(lruk_cache->Get(eight, value), true);
    EXPECT_EQ(lruk_cache->Get(seven, value), true);
    EXPECT_EQ(lruk_cache->Get(six, value), true);
    EXPECT_EQ(lruk_cache->Get(five, value), true);
    EXPECT_EQ(lruk_cache->Get(four, value), true);
    EXPECT_EQ(lruk_cache->Get(three, value), true);
    EXPECT_EQ(lruk_cache->Get(one, value), true);
    
    const std::string nine = "9"; std::string nine_value = "9";
    lruk_cache->Insert(nine, &nine_value);
    EXPECT_EQ(lruk_cache->Get(eight, value), false);

}