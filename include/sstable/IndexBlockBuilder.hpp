#pragma once
#include <string>

#include "../util/Status.hpp"

namespace microkv {

class IndexBlockBuilder {
public:
    IndexBlockBuilder() = default;
    ~IndexBlockBuilder() = default;
public:
    auto Add(const std::string &shortest_segment_key, uint32_t block_offset, uint32_t block_size) -> Status;
    auto Data() const -> std::string;
    void Clear();
private:
    std::string data;

};

} // namespace microkv
