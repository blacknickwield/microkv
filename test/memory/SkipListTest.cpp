#include <iostream>

#include "memory/SkipList.hpp"
#include <gtest/gtest.h>

TEST(SkipListTest, SimpleTest) {
    microkv::SkipList<int> sl;
    sl.Insert(1, "hello");
    sl.Insert(2, "how are you");

    std::string value;
    EXPECT_EQ(true, sl.Find(1, value));
    EXPECT_EQ(value, "hello");
    
    EXPECT_EQ(true, sl.Find(2, value));
    EXPECT_EQ(value, "how are you");

    sl.Erase(2);
    EXPECT_EQ(false, sl.Find(2, value));
}