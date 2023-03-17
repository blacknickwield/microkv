#include "sstable/DataBlockBuilder.hpp"
#include "util/Encoder.hpp"

namespace microkv {
auto DataBlockBuilder::Add(const std::string &key, const std::string &value) -> Status {
    bool first_record = false; // If this record is the first one in the group.
    if (record_num % RECORDS_PER_GROUP == 0) {
        first_record = true;
        group_start_pos.push_back(static_cast<uint32_t>(data.size()));
    }

    if (first_record) {
        encoder::Put32Bit(data, 0); // key shared length
        encoder::Put32Bit(data, key.size()); // key unshared length
        encoder::Put32Bit(data, value.size()); 
        data.append(key); // key unshared part
        data.append(value);
    } else {
        // If this record is not the first one in the group,
        // it need to be compressed.
        uint32_t len = static_cast<uint32_t>(std::min(last_key.length(), key.length()));
        uint32_t shared_length = 0;
        while (shared_length < len) {
            if (last_key[shared_length] != key[shared_length]) break;
            ++shared_length;
        }
        uint32_t unshared_length = static_cast<uint32_t>(key.length() - shared_length);

        encoder::Put32Bit(data, shared_length);
        encoder::Put32Bit(data, unshared_length);
        encoder::Put32Bit(data, value.size());
        data.append(key);
        data.append(value);
    }

    ++record_num;
    last_key = key;
    return {};
}


auto DataBlockBuilder::CreateDataBlock() -> Status {
    uint32_t total_size = static_cast<uint32_t>(data.size());
    // 1 ~ n - 1 group is full.
    for (size_t i = 0; i < group_start_pos.size() - 1; ++i) {
        uint32_t group_record_num = RECORDS_PER_GROUP;
        uint32_t group_size = group_start_pos[i + 1] - group_start_pos[i];
        uint32_t offset = group_start_pos[i];
        encoder::Put32Bit(data, group_record_num);
        encoder::Put32Bit(data, group_size);
        encoder::Put32Bit(data, offset);
    }


    // n group(the last group) may not be full.
    uint32_t last_group_record_num = record_num % RECORDS_PER_GROUP;
    uint32_t last_group_size = total_size - group_start_pos[group_start_pos.size() - 1];
    uint32_t last_offset = group_start_pos[group_start_pos.size() - 1];
    encoder::Put32Bit(data, last_group_record_num);
    encoder::Put32Bit(data, last_group_size);
    encoder::Put32Bit(data, last_offset);

    // Restart point num.
    uint32_t restart_point_num = static_cast<uint32_t>((record_num - 0.5) / 16) + 1;
    encoder::Put32Bit(data, restart_point_num);
    
    return {};
}
} // namespace microk 
