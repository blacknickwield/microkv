#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <cmath>

#include <iostream>

#include "file/FileWriter.hpp"

namespace microkv {

FileWriter::FileWriter(const std::string &path, bool append) : cursor(0) {
    int mode = O_CREAT | O_WRONLY;
    mode |= append ? O_APPEND : O_TRUNC;
    fd = open(path.c_str(), mode);
    if (fd < 0) {
        // TODO
        std::cout << "open fail" << std::endl;
    } else {
        std::cout << "open success" << std::endl;
    }


}

auto FileWriter::Append(const char *data, uint32_t len, bool flush) -> Status {
    assert(cursor <= BUFFER_SIZE);

    uint32_t remaining_size = BUFFER_SIZE - cursor;
    if (remaining_size >= len) {
        memcpy(buffer + cursor, data, len);
        cursor += len;
    } else {
        uint32_t offset = 0;
        memcpy(buffer + cursor, data, remaining_size);
        len -= remaining_size;
        cursor += remaining_size;
        offset += remaining_size;

        // Persist buffer to disk
        FlushBuffer();
        while (len > 0) {
            remaining_size = BUFFER_SIZE - cursor;
            uint32_t append_len = std::min(remaining_size, len);
            memcpy(buffer + cursor, data + offset, append_len);
            cursor += append_len;
            offset += append_len;
            len -= append_len;
            if (IsBufferFull()) {
                FlushBuffer();
            }
        }
    
    }

    // If flush is true, persisting buffer to disk immediatly.
    // Else wait until buffer is full again.
    if (flush) {
        FlushBuffer();
    }

    // TODO
    return {};
}

auto FileWriter::FlushBuffer() -> Status {
    // TODO: write bug. need loop
    write(fd, buffer, BUFFER_SIZE);
    cursor = 0;

    // TODO
    return {};
}
auto FileWriter::IsBufferFull() const -> bool {
    return cursor >= BUFFER_SIZE;
}

}