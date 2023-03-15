#include "memory/Allocator.hpp"

namespace microkv {

Allocator::Allocator() {
    slots.fill(nullptr);
    cursor = static_cast<char *>(malloc(CHUNK_SIZE));
    free_size = CHUNK_SIZE;
}   

void *Allocator::Allocate(int32_t memory_size) {
    if (memory_size <= 0) {
        // TODO: logger
    }

    if (memory_size > MAX_BLOCK_SIZE) {
        return malloc(memory_size);
    }

    auto index = GetIndex(memory_size);
    if (slots[index] == nullptr) {
        FillSLot(index);
    }

    auto *ret = slots[index];
    slots[index]->next = ret;
    return ret->address;
}

int Allocator::GetIndex(int32_t memory_size) const {
    return static_cast<int>(memory_size + 7) / 8 - 1;
}

void Allocator::FillSLot(int32_t index) {
    int32_t block_size = (index + 1) * 8;
    int32_t allocated_size = block_size * BLOCK_CNT;
    if (free_size >= allocated_size) {
        for (size_t i = 0; i < BLOCK_CNT; ++i) {
            auto *node = new BlockNode();
            node->address = cursor + i * block_size;
            node->next = slots[index];
            slots[index] = node;
        }

        free_size -= allocated_size;
        cursor += allocated_size;
    } else if (free_size >= block_size) {
        int32_t cnt = free_size / block_size;
        int32_t new_allocated_size = cnt * block_size;
        for (size_t i = 0; i < cnt; ++i) {
            auto *node = new BlockNode();
            node->address = cursor + i * block_size;
            node->next = slots[index];
            slots[index] = node;
        }

        free_size -= new_allocated_size;
        cursor += new_allocated_size;
    } else {
        NewMemPool();
        FillSLot(index);
    }
    
}

void Allocator::NewMemPool() {
    

    cursor = static_cast<char*>(malloc(CHUNK_SIZE));
    free_size = CHUNK_SIZE;
}

}