#pragma once

#include <mutex>
#include <memory>
#include <ctime>
#include <iostream>
#include <array>
#include <cstring>

#include "SkipListNode.hpp"
#include "Allocator.hpp"

namespace microkv {
template<class K>
class SkipList {
public:
    void Show() const;
public:
    SkipList();
    explicit SkipList(std::shared_ptr<Allocator> allocator);
public:
    class SkipListIterator {
    public:
        explicit SkipListIterator(const SkipList *skipList);
        auto HasNext() const -> bool;
        void Next();
        void Reset();
        auto Key() const -> const K&;
        auto Value() const -> std::string;  
    private:
        const SkipList<K> *skipList;
        SkipListNode<K> *node;
    };
public:
    auto Insert(const K &key, const std::string &value) -> bool;
    auto Find(const K &key, std::string &value) const -> bool;
    auto Erase(const K &key) -> bool;
    auto Count(const K &key) const -> bool;
    const static size_t MAX_LEVEL = 32;
public:
    auto GetMemory() const -> uint32_t;
private:
    auto RandomLevel() const -> size_t;
private:
    size_t length;
    size_t level;
    SkipListNode<K> *head, *tail;
    std::shared_ptr<Allocator> allocator;
    std::mutex mtx;

    uint32_t memory = 0;
};

template<class K>
SkipList<K>::SkipList() : level(0), length(0) {
    srand((unsigned)time(nullptr));
    allocator = std::make_shared<Allocator>();
    K k;
    tail = new SkipListNode<K>(k, 0);
    head = new SkipListNode<K>(k, MAX_LEVEL, tail);
}

template<class K>
SkipList<K>::SkipList(std::shared_ptr<Allocator> allocator) : allocator(allocator), level(0), length(0) {
    srand((unsigned)time(nullptr));
    K k;
    tail = new SkipListNode<K>(k, 0);
    head = new SkipListNode<K>(k, MAX_LEVEL, tail);
}

template<class K>
auto SkipList<K>::RandomLevel() const -> size_t {
    size_t l = 1;
    while (rand() % 2) {
        ++l;
    }
    return l > MAX_LEVEL ? MAX_LEVEL : l;
}

template<class K>
auto SkipList<K>::Insert(const K &key, const std::string &value) -> bool {
    mtx.lock();
    std::array<SkipListNode<K>*, MAX_LEVEL + 1> update;
    auto *current = head;
    for (ssize_t index = level; index >= 0; --index) {
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
    
    auto *insertedNode = new SkipListNode<K>(key, randomLevel, nullptr);
    insertedNode->size = static_cast<uint32_t>(value.length());
    insertedNode->value = static_cast<char*>(allocator->Allocate(insertedNode->size));
    strncpy(insertedNode->value, value.c_str(), insertedNode->size);
    for (int index = randomLevel; index >= 0; --index) {
        insertedNode->forward[index] = update[index]->forward[index];
        update[index]->forward[index] = insertedNode;
    }

    memory += static_cast<uint32_t>(value.length());
    ++length;
    mtx.unlock();
    return true;
}

template<class K>
auto SkipList<K>::Find(const K &key, std::string &value) const -> bool {
    auto *current = head;
    for (ssize_t index = level; index >= 0; --index) {
        while (current->forward[index] != tail && current->forward[index]->key < key) {
            current = current->forward[index];
        }
    }

    current = current->forward[0];
    if (current->key == key) {
        value.assign(current->value, current->size);
        return true;
    }

    return false;
}

template<class K>
auto SkipList<K>::Erase(const K &key) -> bool {
    SkipListNode<K> *update[MAX_LEVEL + 1];    
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

    memory -= current->size;
    allocator->Deallocate(current->value, current->size);
    delete current;
    while (level > 0 && head->forward[level] == tail) {
        --level;
    }
    --length;
    return true;
}

template<class K>
auto SkipList<K>::Count(const K &key) const -> bool {
    std::string value;
    return Find(key, value);
}

template<class K>
auto SkipList<K>::GetMemory() const -> uint32_t {
    return memory;
}

template<class K>
void SkipList<K>::Show() const {
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


template<class K>
SkipList<K>::SkipListIterator::SkipListIterator(const SkipList *skipList) : skipList(skipList) {
    node = skipList->head->forward[0];
}

template<class K>
auto SkipList<K>::SkipListIterator::HasNext() const -> bool {
    return node->forward[0] != skipList->tail;
}

template<class K>
void SkipList<K>::SkipListIterator::Next() {
    node = node->forward[0];
}

template<class K>
void SkipList<K>::SkipListIterator::Reset() {
    node = skipList->head->forward[0];
}

template<class K>
auto SkipList<K>::SkipListIterator::Key() const -> const K& {
    return node->key;
}

template<class K>
auto SkipList<K>::SkipListIterator::Value() const -> std::string {
    std::string value;
    value.assign(node->value, node->size);
    return value;
}

}