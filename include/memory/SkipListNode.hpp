#pragma once

#include <cstddef>
namespace microkv {

template<class K>
class SkipListNode {
public:
    SkipListNode();
    SkipListNode(K _key, size_t _level, SkipListNode *next = nullptr);
    SkipListNode<K> **forward;
    K key;
    char *value;
    uint32_t size;
    size_t level;
};

template<class K>
SkipListNode<K>::SkipListNode() : key{0}, value{nullptr}, size(0), level(0)  {

}

template<class K>
SkipListNode<K>::SkipListNode(K _key, size_t _level, SkipListNode* next) : key(_key), value(nullptr), size(0), level(_level) {
    forward = new SkipListNode*[level + 1];
    for (size_t i = 0; i <= level; ++i) {
        forward[i] = next;
    }
}

}
