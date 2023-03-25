#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "CachePolicy.hpp"
#include "LRU.hpp"
#include "LRUK.hpp"

namespace microkv {
template<class K, class V>
class Cache {
public:
    explicit Cache(const uint32_t capicity);

public:
    void Insert(const K &key, V *value);
    auto Delete(const K &key) -> bool;
    auto Get(const K &key, V *&value) -> bool;

private:
    std::vector<std::shared_ptr<CachePolicy<K, V>>> cache_pool;
    constexpr static uint32_t DEFAULT_CACHE_NUM = 5;
    
};


template<class K, class V>
Cache<K, V>::Cache(const uint32_t capicity) {
    for (uint32_t i = 0; i < DEFAULT_CACHE_NUM; ++i) {
        cache_pool.push_back(std::make_shared<LRU<K, V>>(capicity));
    }
}

template<class K, class V>
void Cache<K, V>::Insert(const K &key, V *value) {
    uint32_t index = std::hash<K>()(key) % DEFAULT_CACHE_NUM;
    cache_pool[index]->Insert(key, value);
}


template<class K, class V>
auto Cache<K, V>::Delete(const K &key) -> bool {
    uint32_t index = std::hash<K>()(key) % DEFAULT_CACHE_NUM;
    return cache_pool[index]->Delete(key);
}

template<class K, class V>
auto Cache<K, V>::Get(const K &key, V *&value) -> bool {
    uint32_t index = std::hash<K>()(key) % DEFAULT_CACHE_NUM;
    return cache_pool[index]->Get(key, value);
}

} // namespace microk 
