#pragma once
#include <cstdint>
#include <string>
namespace microkv::encoder {

void Put8Bit(std::string &dst, uint8_t val);
void Put16Bit(std::string &dst, uint16_t val);
void Put32Bit(std::string &dst, uint32_t val);
void Put64Bit(std::string &dst, uint64_t val);

void Encode8Bit(char *dst, uint8_t val);
void Encode16Bit(char *dst, uint16_t val);
void Encode32Bit(char *dst, uint32_t val);
void Encode64Bit(char *dst, uint64_t val);
} // namespace microkv
