#pragma once
#include <cstdint>
#include <string>
namespace microkv::encoder {

void Put8Bit(std::string &dst, uint8_t val);
void Put16Bit(std::string &dst, uint16_t val);
void Put32Bit(std::string &dst, uint32_t val);
void Put64Bit(std::string &dst, uint64_t val);

uint32_t MurmurHash2(const void *key, uint32_t len);

} // namespace microkv
