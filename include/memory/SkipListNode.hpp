#pragma once

#include <cstddef>
namespace microkv {

template<class K, class V>
class SkipListNode {
public:
    SkipListNode();
    SkipListNode(K _key, V _value, size_t _level, SkipListNode *next = nullptr);
    SkipListNode<K, V> **forward;
// private:
    K key;
    V value;
    size_t level;
};

template<class K, class V>
SkipListNode<K, V>::SkipListNode() : key{0}, value{0}, level(0)  {

}

template<class K, class V>
SkipListNode<K, V>::SkipListNode(K _key, V _value, size_t _level, SkipListNode* next) : key(_key), value(_value), level(_level) {
    forward = new SkipListNode*[level + 1];
    for (size_t i = 0; i <= level; ++i) {
        forward[i] = next;
    }
}

}
