#include <iostream>
#include <memory>
#include <gtest/gtest.h>

#include "memory/Allocator.hpp"

TEST(AllocatorTest, SimpleTest) {
    auto allocator = std::make_shared<microkv::Allocator>();
    auto *p = (int64_t*)allocator->Allocate(8);
    *p = 114514;
    EXPECT_EQ(*p, 114514);
    allocator->Deallocate(p, 8);

    auto *q = (int64_t*)allocator->Allocate(8);
    *q = 1919810;
    EXPECT_EQ(*q, 1919810);
    EXPECT_EQ(*p, 1919810);
    
}