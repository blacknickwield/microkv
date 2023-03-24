#pragma once
#include <string>

namespace microkv {
    
struct Options {
    uint32_t MEMTABLE_SIZE = 4 * 1024 * 1024;
    uint32_t CACHE_SIZE = 4 * 1024;

    std::string SSTABLE_DIR = "./sstable";
};

struct ReadOptions {

};

struct WriteOptions {
    
};


} // namespace smallkv

