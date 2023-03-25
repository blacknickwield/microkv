#pragma once

namespace microkv {

template<class K, class V>
class CachePolicy {
public:
    CachePolicy() = default;
    virtual ~CachePolicy() = default;
    virtual void Insert(const K &key, V *value) = 0;
    virtual auto Delete(const K &key) -> bool = 0;
    virtual auto Get(const K &key, V *&value) -> bool = 0;
private:

};

}