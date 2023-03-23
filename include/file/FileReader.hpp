#pragma once

#include <string>

#include "../util/Status.hpp"

namespace microkv {

class FileReader {
public:
    explicit FileReader(const std::string &file_path);
    auto Read(void *buffer, uint32_t len, uint32_t offset = 0) const -> Status;
private:
    int32_t fd;
};

}