#pragma once

#include <string>
#include "../util/Status.hpp"
#include "../util/Options.hpp"

namespace microkv {

class DB {
public:
    DB() = default;
    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;
    virtual ~DB() {};
public:
    virtual auto Put(const Options &options, const std::string &key, std::string &value) -> Status = 0;
    virtual auto Get(const Options &options, const std::string &key, std::string &value) -> Status= 0;
    virtual auto Delete(const Options &options, const std::string &key) -> Status = 0;
private:

};


} // namespace microk 

