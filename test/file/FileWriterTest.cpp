#include <memory>
#include <string>
#include <cstring>
#include <iostream>

#include "file/FileWriter.hpp"
#include <gtest/gtest.h>

TEST(FileWriterTest, SimpleTest) {
    std::string file_path = "./filewritertest.txt";
    std::shared_ptr<microkv::FileWriter> filewriter = std::make_shared<microkv::FileWriter>(file_path);
    std::string content = "how are you?";
    filewriter->Append(content.c_str(), (uint32_t)strlen(content.c_str()), true);
}