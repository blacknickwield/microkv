#include <iostream>
#include <memory>
#include <cmath>
#include <gtest/gtest.h>

#include "sstable/SSTableBuilder.hpp"
#include "file/FileReader.hpp"
#include "util/Encoder.hpp"

TEST(SSTableBuilderTest, SimpleTest) {
    const std::string file_path = "SSTableBuilderTest.sst";
    uint32_t keysNum = 100;
    double false_positive = 0.1;
    auto fileWriter = std::make_shared<microkv::FileWriter>(file_path);
    auto sstableBuilder = std::make_shared<microkv::SSTableBuilder>(keysNum, false_positive, fileWriter);
    const std::string key = "key0";
    const std::string value = "value0";
    sstableBuilder->Add(key, value);
    sstableBuilder->CreateSSTable();

    auto fileReader = std::make_shared<microkv::FileReader>(file_path);

    char buffer[200];
    fileReader->Read(buffer, 200);

    // Record in DataBlock
    uint32_t shared_key_len = microkv::encoder::Get32Bit(buffer);
    uint32_t unshared_key_len = microkv::encoder::Get32Bit(buffer + 4);
    uint32_t value_len = microkv::encoder::Get32Bit(buffer + 8);

    EXPECT_EQ(shared_key_len, 0);
    EXPECT_EQ(unshared_key_len, 4);
    EXPECT_EQ(value_len, 6);

    std::string k;
    k.assign(buffer + 12, unshared_key_len);
    std::string v;
    v.assign(buffer + 12 + unshared_key_len, value_len);

    EXPECT_EQ(k, key);
    EXPECT_EQ(v, value);

    // Restart in DataBlock
    char *restart = buffer + 12 + unshared_key_len + value_len;
    uint32_t record_num = microkv::encoder::Get32Bit(restart);
    uint32_t offset = microkv::encoder::Get32Bit(restart + 4);
    uint32_t size = microkv::encoder::Get32Bit(restart + 8);
    uint32_t restart_num = microkv::encoder::Get32Bit(restart + 12);

    EXPECT_EQ(record_num, 1);
    EXPECT_EQ(offset, 0);
    EXPECT_EQ(size, 12 + unshared_key_len + value_len);
    EXPECT_EQ(1, restart_num);

    // FilterBlock
    char *filter = restart + 16;

    uint32_t hash_num = microkv::encoder::Get32Bit(filter + 60);
    EXPECT_EQ(hash_num, 2);

    // IndexBlock
    char *index = filter + 64;
    uint32_t shortest_key_size = microkv::encoder::Get32Bit(index);
    std::string shortest_key;
    shortest_key.assign(index + 4, shortest_key_size);
    uint32_t index_offset = microkv::encoder::Get32Bit(index + 4 + shortest_key_size);
    uint32_t index_size = microkv::encoder::Get32Bit(index + 4 + shortest_key_size + 4);
    EXPECT_EQ(shortest_key, "key1");
    EXPECT_EQ(index_offset, 0);
    EXPECT_EQ(index_size, 38);

    // Footer
    char *footer = index + 4 + shortest_key_size + 8;
    uint32_t meta_block_offset = microkv::encoder::Get32Bit(footer);
    uint32_t meta_block_size = microkv::encoder::Get32Bit(footer + 4);
    uint32_t index_block_offset = microkv::encoder::Get32Bit(footer + 8);
    uint32_t index_block_size = microkv::encoder::Get32Bit(footer + 12);

    EXPECT_EQ(meta_block_offset, 38);
    EXPECT_EQ(meta_block_size, 64);
    EXPECT_EQ(index_block_offset, 102);
    EXPECT_EQ(index_block_size, 16);
}