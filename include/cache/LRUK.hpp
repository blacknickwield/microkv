#pragma once

#include <list>
#include <unordered_map>

#include "CachePolicy.hpp"

namespace microkv {

template<class K, class V>
class LRUK : public CachePolicy<K, V> {
public:
    explicit LRUK(uint32_t capicity, uint32_t k);
public:
    void Insert(const K &key, V *value) override;
    auto Get(const K &key, V *&value) -> bool override;
    auto Delete(const K &key) -> bool override;
public:
    struct LRUNode {
        K key;
        V *value;
        uint32_t cnt = 0; // visit count
        LRUNode() = default;
        LRUNode(const K &key, V *value, uint32_t cnt = 0) : key(key), value(value), cnt(cnt) {}
    };
private:
    const uint32_t capicity;
    const uint32_t k;
    using NodePointer = typename std::list<LRUNode*>::iterator;
    std::list<LRUNode*> cache_nodes;
    std::unordered_map<K, NodePointer> cache;

    std::list<LRUNode*> history_nodes;
    std::unordered_map<K, NodePointer> history;
};


template<class K, class V>
LRUK<K, V>::LRUK(uint32_t capicity, uint32_t k) : capicity(capicity), k(k) {

}

template<class K, class V>
void LRUK<K, V>::Insert(const K &key, V *value) {
    if (cache.find(key) != cache.end()) {
        cache_nodes.erase(cache[key]);
        cache_nodes.push_front(new LRUNode(key, value));
        cache[key] = cache_nodes.begin();
        return;
    }

    if (history.find(key) != history.end()) {
        (*history[key])->value = value;
        uint32_t cnt = ++(*history[key])->cnt;

        // Move to cache
        if (cnt >= k) {
            LRUNode *newNode = new LRUNode(key, value);
            history_nodes.erase(history[key]);
            history.erase(key);

            cache_nodes.push_front(newNode);
            cache[key] = cache_nodes.begin();
        }
        return;
    }

    uint32_t size = static_cast<uint32_t>(history.size() + cache.size());
    if (size > capicity) {
        if (!history.empty()) {
            auto *deleteNode = history_nodes.back();
            history.erase(deleteNode->key);
            history_nodes.remove(deleteNode);
        } else {
            auto *deleteNode = cache_nodes.back();
            cache.erase(deleteNode->key);
            cache_nodes.remove(deleteNode);
        }
    }

    LRUNode *newNode = new LRUNode(key, value, 1);
    history_nodes.push_front(newNode);
    history[key] = history_nodes.begin();
}


template<class K, class V>
auto LRUK<K, V>::Get(const K &key, V *&value) -> bool {
    if (cache.find(key) != cache.end()) {
        auto it = cache[key];
        LRUNode *newNode = new LRUNode(key, (*it)->value);
        value = newNode->value;
        cache_nodes.push_front(newNode);
        cache_nodes.erase(cache[key]);
        cache[key] = cache_nodes.begin();

        return true;
    }

    if (history.find(key) != history.end()) {
        uint32_t cnt = ++(*history[key])->cnt;
        value = (*history[key])->value;

        // Move to cache
        if (cnt >= k) {
            LRUNode *newNode = new LRUNode(key, (*history[key])->value);
            history_nodes.erase(history[key]);
            history.erase(key);

            cache_nodes.push_front(newNode);
            cache[key] = cache_nodes.begin();
        }

        return true;
    }

    value = nullptr;
    return false;

}


template<class K, class V>
auto LRUK<K, V>::Delete(const K &key) -> bool {
    if (cache.find(key) != cache.end()) {
        cache_nodes.erase(cache[key]);
        cache.erase(key);
        return true;
    }

    if (history.find(key) != history.end()) {
        history_nodes.erase(history[key]);
        history.erase(key);
        return true;
    }

    return false;
}

}