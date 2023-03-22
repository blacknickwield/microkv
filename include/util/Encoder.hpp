#pragma once
#include <cstdint>
#include <string>
namespace microkv::encoder {

void Put8Bit(std::string &dst, uint8_t val);
void Put16Bit(std::string &dst, uint16_t val);
void Put32Bit(std::string &dst, uint32_t val);
void Put64Bit(std::string &dst, uint64_t val);


uint8_t Get8Bit(const char *data);
uint16_t Get16Bit(const char *data);
uint32_t Get32Bit(const char *data);
uint64_t Get64Bit(const char *data);

uint32_t MurmurHash2(const void *key, uint32_t len);

} // namespace microkv
