#pragma once

#include <shared_mutex>

#include "DB.hpp"
#include "../memory/MemTable.hpp"
#include "../cache/Cache.hpp"

namespace microkv {
class DBImpl : public DB {
public:
    explicit DBImpl(const Options &options);
    ~DBImpl();
public:
    auto Put(const WriteOptions &options, const std::string &key, const std::string &value) -> Status override;
    auto Get(const ReadOptions &options, const std::string &key, std::string &value) -> Status override;
    auto Delete(const WriteOptions &options, const std::string &key) -> Status override;
private:
    const Options options;
    std::shared_ptr<MemTable> memTable;
    std::shared_ptr<Allocator> allocator;
    std::shared_ptr<Cache<std::string, std::string>> cache;
    std::shared_mutex rwlock;
};

}
