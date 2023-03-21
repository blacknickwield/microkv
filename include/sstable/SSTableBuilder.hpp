#pragma once

#include <memory>

#include "DataBlockBuilder.hpp"
#include "FilterBlockBuilder.hpp"
#include "FooterBuilder.hpp"
#include "IndexBlockBuilder.hpp"
#include "MetaBlockBuilder.hpp"
#include "BlockInfo.hpp"
#include "../file/FileWriter.hpp"
#include "../util/Status.hpp"

namespace microkv {

class SSTableBuilder {
public:
    SSTableBuilder(uint32_t keysNum, double false_positive, std::shared_ptr<FileWriter> fileWriter);
public:
    auto Add(const std::string &key, const std::string &value) -> Status;
    auto CreateSSTable() -> Status;
private:
    auto FindShortestSegmentKey(const std::string &key1, const std::string &key2) -> std::string;
private:
    uint32_t keysNum;
    double false_positive;
    std::shared_ptr<DataBlockBuilder> dataBlockBuilder;
    std::shared_ptr<FilterBlockBuilder> filterBlockBuilder;
    std::shared_ptr<FooterBuilder> footerBuilder;
    std::shared_ptr<IndexBlockBuilder> indexBlockBuilder;

    std::shared_ptr<FileWriter> fileWriter;

    std::string last_key;

    std::string indexBlock;
    BlockInfo filterBlockInfo{0, 0};
    BlockInfo indexBlockInfo{0, 0};
};

} // namespace microk 