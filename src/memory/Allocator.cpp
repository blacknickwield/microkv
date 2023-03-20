#include "memory/Allocator.hpp"

namespace microkv {

Allocator::Allocator() {
    slots.fill(nullptr);
    cursor = static_cast<char *>(malloc(CHUNK_SIZE));
    free_size = CHUNK_SIZE;
}   

auto Allocator::Allocate(int32_t memory_size) -> void* {
    if (memory_size <= 0) {
        // TODO: logger
    }

    // If requested memory is larger than 4KB,
    // call malloc() to allocate space for it.
    if (memory_size > MAX_BLOCK_SIZE) {
        return malloc(memory_size);
    }

    auto index = GetIndex(memory_size);
    // If slots[index] is nullptr,
    // request a memory block from memory pool and mount it to slots[index]
    if (slots[index] == nullptr) {
        FillSLot(index);
    }

    auto *ret = slots[index];
    // Ensure that if slots[index] exists, it is always free.
    // So it can be allocated directly.
    slots[index] = ret->next;
    // return ret->address;
    return ret;
}


void Allocator::Deallocate(void *p, int32_t memory_size) {
    if (memory_size <= 0) {
        // TODO: laogger
    }

    // Memory larger than 4KB is alocated by malloc().
    // So call free() to deallocate it.
    if (memory_size > MAX_BLOCK_SIZE) {
        free(p);
        return;
    }

    // Data on it don't really need to be erased.
    // In fact, just move it to slots[index].
    // So it can be considered unused.
    auto index = GetIndex(memory_size);
    auto *node = static_cast<BlockNode*>(p);
    node->next = slots[index];
    slots[index] = node;
}

void Allocator::Reallocate(void *&p, int32_t old_size, int32_t new_size) {
    Deallocate(p, old_size);
    p = Allocate(new_size);
}


int Allocator::GetIndex(int32_t memory_size) const {
    return static_cast<int>(memory_size + 7) / 8 - 1;
}

void Allocator::FillSLot(int32_t index) {
    int32_t block_size = (index + 1) * 8;
    int32_t allocated_size = block_size * BLOCK_CNT;
    if (free_size >= allocated_size) {
        for (size_t i = 0; i < BLOCK_CNT; ++i) {
            // auto *node = new BlockNode();
            // node->address = cursor + i * block_size;
            auto *node = reinterpret_cast<BlockNode*>(cursor + i * block_size);
            if (i == 0) {
                node->next = nullptr;
            }
            node->next = slots[index];
            slots[index] = node;
        }

        free_size -= allocated_size;
        cursor += allocated_size;
    } else if (free_size >= block_size) {
        int32_t cnt = free_size / block_size;
        int32_t new_allocated_size = cnt * block_size;
        for (size_t i = 0; i < cnt; ++i) {
            // auto *node = new BlockNode();
            // node->address = cursor + i * block_size;
             auto *node = reinterpret_cast<BlockNode*>(cursor + i * block_size);
            if (i == 0) {
                node->next = nullptr;
            }
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