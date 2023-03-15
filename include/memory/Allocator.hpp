#pragma once

#include <memory>
#include <cstdint>
#include <array>

namespace microkv {

struct BlockNode {
    void *address; // The start address of the block memory. Its value is null if it is free.
    BlockNode *next;
};

class Allocator {
public:
    Allocator();
    ~Allocator() = default;
    void *Allocate(int32_t size);
private:
    int GetIndex(int32_t memory_size) const;
    void FillSLot(int32_t index);
    void NewMemPool();
private:
    constexpr static int32_t SLOT_NUM = 512; 
    constexpr static int32_t CHUNK_SIZE = 4 * 1024 * 1024;
    constexpr static int32_t BLOCK_CNT = 10;
    constexpr static int32_t MAX_BLOCK_SIZE = 4 * 1024; // If required size is larger than MAX_BLOCK_SIZE(4KB), call malloc() directly.
private:
    std::array<BlockNode*, SLOT_NUM> slots;
    char *cursor = nullptr;
    int32_t free_size = 0;
};

}
