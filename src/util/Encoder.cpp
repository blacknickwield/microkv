#include <cstdlib>

#include "util/Encoder.hpp"

namespace microkv::encoder {

void Encode8Bit(char *dst, uint8_t val) {
    *dst = val & 0xff;
}

void Encode16Bit(char *dst, uint16_t val) {
    for (size_t i = 0; i < 2; ++i) {
        dst[i] = (val >> (8 * i)) & 0xff;
    }
}

void Encode32Bit(char *dst, uint32_t val) {
    for (size_t i = 0; i < 4; ++i) {
        dst[i] = (val >> (8 * i)) & 0xff;
    }
}

void Encode64Bit(char *dst, uint64_t val) {
    for (size_t i = 0; i < 8; ++i) {
        dst[i] = ((val >> (8 * i))) & 0xff; 
    }
}

void Put8Bit(std::string &dst, uint8_t val) {
    char *buf = static_cast<char*>(malloc(sizeof(val)));
    Encode8Bit(buf, val);
    dst.append(buf, sizeof(val));
    free(buf);
}

void Put16Bit(std::string &dst, uint16_t val) {
    char *buf = static_cast<char*>(malloc(sizeof(val)));
    Encode16Bit(buf, val);
    dst.append(buf, sizeof(val));
    free(buf);
}

void Put32Bit(std::string &dst, uint32_t val) {
    char *buf = static_cast<char*>(malloc(sizeof(val)));
    Encode32Bit(buf, val);
    dst.append(buf, sizeof(val));
    free(buf);
}

void Put64Bit(std::string &dst, uint64_t val) {
    char *buf = static_cast<char*>(malloc(sizeof(val)));
    Encode64Bit(buf, val);
    dst.append(buf, sizeof(val));
    free(buf);
}



} // namespace microk
