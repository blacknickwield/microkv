#include <filesystem>

#include "sstable/SSTableReader.hpp"
#include "util/Encoder.hpp"

namespace microkv {

SSTableReader::SSTableReader(const std::string &file_path) {
    std::filesystem::path path = file_path;
    ifs = path;
}

void SSTableReader::Read() {
    ReadFile();
    ReadFooter();
    ReadFilterBlock();
    ReadIndexBlock();
}

void SSTableReader::ReadFile() {
    std::string buf;
    while (getline(ifs, buf)) {
        content.append(buf);
        buf.clear();
    }
}

void SSTableReader::ReadFooter() {
    uint32_t file_size = static_cast<uint32_t>(content.size());
    char *sstable = content.data();

    filterBlockInfo.offset = encoder::Get32Bit(sstable + file_size - 16);
    filterBlockInfo.size = encoder::Get32Bit(sstable + file_size - 12);

    indexBlockInfo.offset = encoder::Get32Bit(sstable + file_size - 8);
    indexBlockInfo.size = encoder::Get32Bit(sstable + file_size - 4);

}

void SSTableReader::ReadIndexBlock() {
    char *sstable = content.data();
    char *indexBlock = sstable + indexBlockInfo.offset;
    char *indexBlockEnd = indexBlock + indexBlockInfo.size;

    while (indexBlock != indexBlockEnd) {
        IndexBlock block;
        uint32_t shortest_key_size = encoder::Get32Bit(indexBlock);
        std::string shortest_key;
        shortest_key.assign(indexBlock + 4, shortest_key_size);
        block.shortest_key = std::move(shortest_key);
        block.offset = encoder::Get32Bit(indexBlock + 4 + shortest_key_size);
        block.size = encoder::Get32Bit(indexBlock + 4 + shortest_key_size + 4);
        
        indexBlocks.emplace_back(block);
        indexBlock += 12 + shortest_key_size;
    }
}

void SSTableReader::ReadFilterBlock() {
    char *sstable = content.data();
    std::string bitSet = std::string(sstable + filterBlockInfo.offset, filterBlockInfo.size - 4);
    uint32_t hashNums = encoder::Get32Bit(sstable + filterBlockInfo.offset + filterBlockInfo.size - 4);

    filter = std::make_shared<BloomFilter>(hashNums, bitSet);
}

auto SSTableReader::Find(const std::string &key, std::string &value) -> bool {
    if (!Exist(key)) {
        return false;
    }

    // Find the DataBlock index
    int32_t index = FindDataBlockIndex(key);
    if (index < 0) {
        return false;
    }

    // Find in the DataBlock
    return FindInDataBlock(key, value, static_cast<uint32_t>(index));
}

auto SSTableReader::Exist(const std::string &key) const -> bool {
    return filter->Exist(key);
}

auto SSTableReader::FindDataBlockIndex(const std::string &key) const -> int32_t {
    for (int32_t i = 0; i < indexBlocks.size(); ++i) {
        if (key < indexBlocks[i].shortest_key) {
            return i;
        }
    }
    return -1;
}

auto SSTableReader::FindInDataBlock(const std::string &key, std::string &value, uint32_t indexBlockIndex) -> bool {
    char *sstable = content.data();
    const auto &indexBlock = indexBlocks[indexBlockIndex];
    char *dataBlock = sstable + indexBlock.offset;

    uint32_t restart_num = encoder::Get32Bit(dataBlock + indexBlock.size - 4);
    // char *restart_start = dataBlock + indexBlock.size;
    char *restart_start = dataBlock + indexBlock.size - 4 - 12 * restart_num;

    // Find the group
    int32_t record_num = -1;
    int32_t group_offset = -1;
    int32_t group_size = -1;
    std::string fullKey;

    for (uint32_t i = 0; i < restart_num; ++i) {
        char *restart = restart_start + i * 12;
        record_num = static_cast<int32_t>(encoder::Get32Bit(restart));
        group_offset = static_cast<int32_t>(encoder::Get32Bit(restart + 4));
        group_size = static_cast<int32_t>(encoder::Get32Bit(restart + 8));

        fullKey = FindFullKey(dataBlock + group_offset);
        if (key < fullKey) {
            break;
        }
    }

    // False positive
    if (record_num < 0) {
        return false;
    }

    // Find in the group
    char *record = dataBlock + group_offset;
    for (int32_t i = 0; i < record_num; ++i) {
        uint32_t shared_key_len = encoder::Get32Bit(record);
        uint32_t unshared_key_len = encoder::Get32Bit(record + 4);
        uint32_t value_len = encoder::Get32Bit(record + 8);

        std::string unshared_key;
        unshared_key.assign(record + 12, unshared_key_len);
        std::string value_content;
        value_content.assign(record + 12 + unshared_key_len, value_len);

        std::string completeKey = fullKey.substr(0, shared_key_len) + unshared_key;
        if (completeKey == key) {
            value = value_content;
            return true;
        }
        record += 12 + unshared_key_len + value_len;
    }

    return false;
}

auto SSTableReader::FindFullKey(char *record) -> std::string {
    uint32_t shared_len = encoder::Get32Bit(record);
    uint32_t unshared_len = encoder::Get32Bit(record + 4);

    std::string fullKey;
    fullKey.assign(record + 12, unshared_len);
    return fullKey;
}

}