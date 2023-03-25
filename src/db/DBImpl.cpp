#include "db/DBImpl.hpp"
#include "sstable/SSTableBuilder.hpp"
#include "sstable/SSTableReader.hpp"

#include <iostream>
#include <filesystem>

namespace microkv {

DBImpl::DBImpl(const Options &options) : options(options) {
    allocator = std::make_shared<Allocator>();
    memTable = std::make_shared<MemTable>(allocator);
    cache = std::make_shared<Cache<std::string, std::string>>(options.CACHE_SIZE);
}

DBImpl::~DBImpl() {

}

auto DBImpl::Put(const Options &woptions, const std::string &key, std::string &value) -> Status {
    // Write lock
    std::unique_lock<std::shared_mutex> wlock(rwlock); 

    // Write to WAL
    // TODO
    
    // Write to MemTable
    if (memTable->Exist(key)) {
        memTable->Update(key, value);
    } else {
        memTable->Add(key, value);
    }

    // Write to cache
    std::string *value_content = new std::string(value);
    // cache->Insert(key, &value);
    cache->Insert(key, value_content);

    if (memTable->GetMemory() > options.MEMTABLE_SIZE) {
        const std::string file_path = options.SSTABLE_DIR + "sstable0.sst";
        auto fileWriter = std::make_shared<FileWriter>(file_path);
        uint32_t keys_num = memTable->GetSize();
        double false_positive = 0.01;

        auto sstableBuilder = std::make_shared<SSTableBuilder>(keys_num, false_positive, fileWriter);
        memTable->Persist(sstableBuilder);
        memTable = std::make_shared<MemTable>(allocator);
    }

    return DBStatus::SUCCESS;
}

auto DBImpl::Get(const Options &roptions, const std::string &key, std::string &value) -> Status {
    // Read lock
    std::shared_lock<std::shared_mutex> rlock(rwlock);

    // Read cache
    auto *addr = &value;
    if (cache->Get(key, addr)) {
        value = *addr;
        return DBStatus::SUCCESS;
    }

    // Read MemTable
    if (memTable->Get(key, value)) {
        return DBStatus::SUCCESS;
    }

    // Read sstable file
    const std::string file_path = options.SSTABLE_DIR +"sstable0.sst";
    std::filesystem::path path = file_path;
    if (!std::filesystem::exists(path)) {
        return DBStatus::NotFound;
    }
    auto sstableReader = std::make_shared<SSTableReader>(file_path);

    sstableReader->Read();


    if (!sstableReader->Find(key, value)) {
        return DBStatus::NotFound;
    }

    // Write to cache
    std::string *cache_value = new std::string(value);
    cache->Insert(key, cache_value);

    return DBStatus::SUCCESS;
}

auto DBImpl::Delete(const Options &woptions, const std::string &key) -> Status {
    // Write lock
    std::unique_lock<std::shared_mutex> wlock(rwlock);

    // Delete MemTable
    if (!memTable->Exist(key)) {
        return DBStatus::NotFound;
    }

    memTable->Delete(key);


    // Delete cache
    cache->Delete(key);

    return DBStatus::SUCCESS;
}

}