#include "sstable/IndexBlockBuilder.hpp"
#include "util/Encoder.hpp"

namespace microkv {

auto IndexBlockBuilder::Add(const std::string &shortest_segment_key, uint32_t block_offset, uint32_t block_size) -> Status {
    encoder::Put32Bit(data, static_cast<uint32_t>(shortest_segment_key.size()));
    data.append(shortest_segment_key);
    encoder::Put32Bit(data, block_offset);
    encoder::Put32Bit(data, block_size);
    
    return {};
}

}