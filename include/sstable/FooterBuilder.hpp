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
    auto Data() const -> const std::string&;
    auto Size() const -> uint32_t;
    void Clear();
private:
    std::string data;
};

}