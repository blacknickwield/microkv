#include "sstable/FooterBuilder.hpp"
#include "util/Encoder.hpp"


namespace microkv {

auto FooterBuilder::Add(uint32_t meta_block_offset, uint32_t meta_block_size, 
                        uint32_t index_block_offset, uint32_t index_block_size) -> Status {
    encoder::Put32Bit(data, meta_block_offset);
    encoder::Put32Bit(data, meta_block_size);
    encoder::Put32Bit(data, index_block_offset);
    encoder::Put32Bit(data, index_block_size);
}

}