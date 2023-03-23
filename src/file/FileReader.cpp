#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include "file/FileReader.hpp"

namespace microkv {

FileReader::FileReader(const std::string &file_path) {
    int mode = O_RDONLY;
    fd = open(file_path.c_str(), mode);
    if (fd < 0) {
        // TODO
        std::cout << "open fail" << std::endl;
    } else {
        std::cout << "open success" << std::endl;
    }
}

auto FileReader::Read(void *buffer, uint32_t len, uint32_t offset) const -> Status {
    auto total = pread(fd, buffer, len, offset);
    // TODO
    return {};
}

}