#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "BlockInfo.hpp"
#include "../filter/BloomFilter.hpp"

namespace microkv {

class SSTableReader {
public:
    explicit SSTableReader(const std::string &file_path);
public:
    struct IndexBlock {
        std::string shortest_key;
        uint32_t offset = 0;
        uint32_t size = 0;
    };
    void Read();
    auto Find(const std::string &key, std::string &value) -> bool;
private:
    void ReadFile();
    void ReadFooter();
    void ReadIndexBlock();
    void ReadFilterBlock();
    auto Exist(const std::string &key) const -> bool;
    auto FindDataBlockIndex(const std::string &key) const -> int32_t;
    auto FindInDataBlock(const std::string &key, std::string &value, uint32_t indexBlockIndex) -> bool;
    auto FindFullKey(char *dataBlock) -> std::string;
private:
    std::string content;
    std::vector<IndexBlock> indexBlocks;
    std::ifstream ifs;
    std::shared_ptr<BloomFilter> filter;
    
    BlockInfo filterBlockInfo;
    BlockInfo indexBlockInfo;
};

}