#pragma once

#include <cstdint>
#include <string>

#include "../util/Status.hpp"

namespace microkv {
    
class FileWriter {
public:
    explicit FileWriter(const std::string &path, bool append = true);
public:
    auto Append(const char *data, uint32_t len, bool flush) -> Status;
private:
    auto IsBufferFull() const -> bool;
    auto FlushBuffer() -> Status;
private:
    constexpr static uint32_t BUFFER_SIZE = 64 * 1024; // Buffer size is 64KB.
    char buffer[BUFFER_SIZE];
    uint32_t cursor = 0;
    int32_t fd;
};

} // namespace microk 