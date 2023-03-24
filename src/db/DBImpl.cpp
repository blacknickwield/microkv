#include "db/DBImpl.hpp"
#include "sstable/SSTableBuilder.hpp"
#include "sstable/SSTableReader.hpp"

namespace microkv {

DBImpl::DBImpl(const Options &options) : options(options) {
    allocator = std::make_shared<Allocator>();
    memTable = std::make_shared<MemTable>(allocator);
    cache = std::make_shared<Cache<std::string, std::string>>(options.CACHE_SIZE);
}

DBImpl::~DBImpl() {

}

auto DBImpl::Put(const WriteOptions &woptions, const std::string &key, const std::string &value) -> Status {
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
    cache->Insert(key, &value);

    if (memTable->GetMemory() > options.MEMTABLE_SIZE) {
        auto sstableBuilder = std::make_shared<SSTableBuilder>();
        memTable->Persist(sstableBuilder);
        memTable = std::make_shared<MemTable>(allocator);
    }

    return DBStatus::SUCCESS;
}

auto DBImpl::Get(const ReadOptions &roptions, const std::string &key, std::string &value) -> Status {
    // Read lock
    std::shared_lock<std::shared_mutex> rlock(rwlock);

    // Read cache
    if (cache->Get(key, &value)) {
        return DBStatus::SUCCESS;
    }

    // Read MemTable
    if (memTable->Get(key, value)) {
        return DBStatus::SUCCESS;
    }

    // Read sstable file
    const std::string file_path = options.SSTABLE_DIR +"sstable0.sst";
    auto sstableReader = std::make_shared<SSTableReader>(file_path);
    sstableReader->Read();

    if (!sstableReader->Find(key, value)) {
        return DBStatus::NotFound;
    }

    // Write to cache
    const std::string *cache_value = new std::string(value);
    cache->Insert(key, cache_value);

    return DBStatus::SUCCESS;
}

auto DBImpl::Delete(const WriteOptions &woptions, const std::string &key) -> Status {
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