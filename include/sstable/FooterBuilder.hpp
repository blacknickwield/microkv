#pragma once

#include <string>

#include "../util/Status.hpp"

namespace microkv {

class FooterBuilder {
public:
    FooterBuilder() = default;
    ~FooterBuilder() = default;
public:
    auto Add(uint32_t meta_block_offset, uint32_t meta_block_size, uint32_t index_block_offset, uint32_t index_block_size) -> Status;
private:
    std::string data;
};

}