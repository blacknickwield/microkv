#include "sstable/FooterBuilder.hpp"
#include "util/Encoder.hpp"


namespace microkv {

auto FooterBuilder::Add(uint32_t meta_block_offset, uint32_t meta_block_size, 
                        uint32_t index_block_offset, uint32_t index_block_size) -> Status {
    encoder::Put32Bit(data, meta_block_offset);
    encoder::Put32Bit(data, meta_block_size);
    encoder::Put32Bit(data, index_block_offset);
    encoder::Put32Bit(data, index_block_size);
    return {};
}


auto FooterBuilder::Data() const -> const std::string& {
    return data;
}

auto FooterBuilder::Size() const -> uint32_t {
    return static_cast<uint32_t>(data.length());
}

void FooterBuilder::Clear() {
    data.clear();
}
}