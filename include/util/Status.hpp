#pragma once
#include <string>
#include <cstdint>

namespace microkv {
struct Status {
    uint32_t code;
    const char *msg;
    auto operator==(const Status& other) -> bool {
        return code == other.code;
    }

    auto operator!=(const Status& other) -> bool {
        return code != other.code;
    }
};

struct DBStatus {
    DBStatus() = delete;
    ~DBStatus() = delete;

    static constexpr Status SUCCESS = {1, "success"};
    static constexpr Status NotFound = {2, "not found"};

};

}

