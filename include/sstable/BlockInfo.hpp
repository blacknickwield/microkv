#pragma once
#include <cstdint>

namespace microkv {

struct BlockInfo {
    uint32_t offset;
    uint32_t size;

    void Clear() {
        offset = size = 0;
    }
};

}