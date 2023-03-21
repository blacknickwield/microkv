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

uint32_t MurmurHash2(const void *key, uint32_t len) {
        // 'm' and 'r' are mixing constants generated offline.
        // They're not really 'magic', they just happen to work well.
        static constexpr uint32_t seed = 0xbc451d34;
        static constexpr uint32_t m = 0x5bd1e995;
        static constexpr uint32_t r = 24;

        // Initialize the hash to a 'random' value

        uint32_t h = seed ^ len;

        // Mix 4 bytes at a time into the hash

        const uint8_t *data = (const unsigned char *) key;

        while (len >= 4) {
            uint32_t k = *(uint32_t *) data;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            len -= 4;
        }

        // Handle the last few bytes of the input array

        switch (len) {
            case 3:
                h ^= data[2] << 16;
            case 2:
                h ^= data[1] << 8;
            case 1:
                h ^= data[0];
                h *= m;
        };

        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

} // namespace microk
