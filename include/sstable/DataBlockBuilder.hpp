#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../util/Status.hpp"

namespace microkv {
class DataBlockBuilder {
public:
    DataBlockBuilder() = default;
    ~DataBlockBuilder() = default;
public:
    auto Add(const std::string &key, const std::string &value) -> Status;
    auto CreateDataBlock() -> Status;
    auto IsFull() const -> bool;
    auto Data() const -> const std::string&;
    auto Size() const -> uint32_t;
    auto Empty() const -> bool;
    void Clear();
private:
    std::string data;
    std::vector<uint32_t> group_start_pos; // Record where each group starts from in data.
    uint32_t record_num; // The number of records in this data block.
    std::string last_key; // The last key. Used for calculate shared length.
    const static uint32_t RECORDS_PER_GROUP = 16; // Every 16 records form a group. 
    const static uint32_t DEFAULT_DATABLOCK_SIZE = 4 * 1024; // 4KB
};
 
} // namespace microkv

