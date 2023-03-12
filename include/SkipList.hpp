#include <mutex>
#include <ctime>
#include <iostream>

#include "SkipListNode.hpp"

namespace microkv {
template<class K, class V>
class SkipList {
public:
    void Show() const;
public:
    SkipList();
public:
    auto Insert(const K &key, const V &value) -> bool;
    auto Erase(const K &key) const -> bool;
    auto Find(const K &key, V &value) const -> bool;
    auto Erase(const K &key) -> bool;
    auto Count(const K &key) const -> bool;
    const static size_t MAX_LEVEL = 32;
private:
    auto RandomLevel() const -> size_t;
private:
    size_t length;
    size_t level;
    SkipListNode<K, V> *head, *tail;
    std::mutex mtx;
};

template<class K, class V>
SkipList<K, V>::SkipList() : level(0), length(0) {
    srand((unsigned)time(nullptr));
    K k; V v;
    tail = new SkipListNode<K, V>(k, v, 0);
    head = new SkipListNode<K, V>(k, v, MAX_LEVEL, tail);
}

template<class K, class V>
auto SkipList<K, V>::RandomLevel() const -> size_t {
    size_t l = 1;
    while (rand() % 2) {
        ++l;
    }
    return l > MAX_LEVEL ? MAX_LEVEL : l;
}

template<class K, class V>
auto SkipList<K, V>::Insert(const K &key, const V &value) -> bool {
    mtx.lock();
    SkipListNode<K, V> *update[MAX_LEVEL + 1];
    auto *current = head;
    for (int index = level; index >= 0; --index) {
        while (current->forward[index] != tail && current->forward[index]->key < key) {
            current = current->forward[index];
        }
        update[index] = current;
    }
    // The first node whose key is equal to or larger than inserted key in level 0
    // current = update[0];
    current = current->forward[0];
    if (current != tail && current->key == key) {
        mtx.unlock();
        return false;
    }


    size_t randomLevel = RandomLevel();
    if (randomLevel > level) {
        randomLevel = ++level;
        update[randomLevel] = head;
    }
    
    auto *insertedNode = new SkipListNode<K, V>(key, value, randomLevel);
    for (int index = randomLevel; index >= 0; --index) {
        insertedNode->forward[index] = update[index]->forward[index];
        update[index]->forward[index] = insertedNode;
    }

    ++length;
    mtx.unlock();
    return true;
}

template<class K, class V>
auto SkipList<K, V>::Erase(const K &key) const -> bool {

    return false;
}

template<class K, class V>
auto SkipList<K, V>::Find(const K &key, V &value) const -> bool {
    auto *current = head;
    for (int index = level; index >= 0; --index) {
        while (current->forward[index] != tail && current->forward[index]->key < key) {
            current = current->forward[index];
        }
    }

    current = current->forward[0];
    if (current->key == key) {
        value = current->value;
        return true;
    }

    return false;
}

template<class K, class V>
auto SkipList<K, V>::Erase(const K &key) -> bool {
    SkipListNode<K, V> *update[MAX_LEVEL + 1];    
    auto *current = head;
    for (ssize_t index = level; index >= 0; --index) {
        while (current->forward[index] != tail && current->forward[index]->key < key) {
            current = current->forward[index];
        }
        update[index] = current;
    }

    current = current->forward[0];
    if (current == tail || current->key != key) {
        return false;
    }

    for (size_t index = 0; index <= level; ++index) {
        if (update[index]->forward[index] != current) {
            break;
        } 
        update[index]->forward[index] = current->forward[index];
    }

    delete current;
    while (level > 0 && head->forward[level] == tail) {
        --level;
    }
    --length;
    return true;
}

template<class K, class V>
auto SkipList<K, V>::Count(const K &key) const -> bool {
    V value;
    return Find(key, value);
}

template<class K, class V>
void SkipList<K, V>::Show() const {
    using std::cout;
    using std::endl;
    cout << level << endl;
    for (int index = level; index >= 0; --index) {
        for (auto *p = head->forward[index]; p != tail; p = p->forward[index]) {
            cout << '(' <<p->key << ", " << p->value << ')' << ' ';
        }
        cout << endl;
    }
}

}