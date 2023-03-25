#pragma once

#include <cstdint>
#include <unordered_map>
#include <list>

#include <iostream>

#include "CachePolicy.hpp"

namespace microkv {
    
template<class K, class V>
class LRU : public CachePolicy<K, V> {
public:
    explicit LRU(const uint32_t capicity);
public:
    void Insert(const K &key, V* value) override;
    auto Delete(const K &key) -> bool override;
    auto Get(const K &key, V *&value) -> bool override;
public:
    struct LRUNode {
        K key;
        V *value;
        LRUNode() = default;
        LRUNode(const K &key, V *value) : key(key), value(value) {}
    };
private:
    const uint32_t capicity;
    using NodePointer = typename std::list<LRUNode*>::iterator;
    std::list<LRUNode*> nodes;
    std::unordered_map<K, NodePointer> cache; 
};


template<class K, class V>
LRU<K, V>::LRU(const uint32_t capicity) : capicity(capicity) {

}


template<class K, class V>
void LRU<K, V>::Insert(const K &key, V *value) {
    if (auto it = cache.find(key); it == cache.end()) {
        LRUNode *newNode = new LRUNode(key, value);

        nodes.push_front(newNode);
        cache[key] = nodes.begin();

        uint32_t size = nodes.size();
        if (size > capicity) {
            auto *deleteNode = nodes.back();
            cache.erase(deleteNode->key);
            nodes.pop_back();
            delete deleteNode;
        }
    } else {
        nodes.erase(cache[key]);
        nodes.push_front(new LRUNode(key, value));

        cache[key] = nodes.begin();
    }
}


template<class K, class V>
auto LRU<K, V>::Delete(const K &key) -> bool {
    if (auto it = cache.find(key); it != cache.end()) {
        nodes.erase(cache[key]);
        cache.erase(key);
        return true;
    } else {
        return false;
    }
}

template<class K, class V>
auto LRU<K, V>::Get(const K &key, V *&value) -> bool {
    if (auto it = cache.find(key); it != cache.end()) {
        value = (*cache[key])->value;
        nodes.erase(cache[key]);
        nodes.push_front(new LRUNode(key, value));
        cache[key] = nodes.begin();
        return true;
    } else {
        value = nullptr;
        return false;
    }
}

} // namespace microk class LRU : public