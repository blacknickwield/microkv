#include "sstable/SSTableBuilder.hpp"

namespace microkv {

SSTableBuilder::SSTableBuilder(uint32_t keysNum, double false_positive, std::shared_ptr<FileWriter> fileWriter) : keysNum(keysNum), false_positive(false_positive), fileWriter(fileWriter) {
    dataBlockBuilder = std::make_shared<DataBlockBuilder>();
    indexBlockBuilder = std::make_shared<IndexBlockBuilder>();
    footerBuilder = std::make_shared<FooterBuilder>();
    filterBlockBuilder = std::make_shared<FilterBlockBuilder>(keysNum, false_positive);
}


auto SSTableBuilder::Add(const std::string &key, const std::string &value) -> Status {
    if (key.empty()) {
        return {};
    }

    dataBlockBuilder->Add(key, value);
    filterBlockBuilder->Add(key);

    if (dataBlockBuilder->IsFull()) {
        dataBlockBuilder->CreateDataBlock();
        fileWriter->Append(dataBlockBuilder->Data().c_str(), dataBlockBuilder->Size(), true);

        auto shortest_segment_key = FindShortestSegmentKey(last_key, key);

        uint32_t block_size = dataBlockBuilder->Size();
        // TODO: The order below
        uint32_t block_offset = filterBlockInfo.offset;
        filterBlockInfo.offset += dataBlockBuilder->Size();
        indexBlockBuilder->Add(shortest_segment_key, block_offset, block_size);
    
        indexBlock.append(indexBlockBuilder->Data());
        indexBlockBuilder->Clear();

        dataBlockBuilder->Clear();
    }

    last_key = key;
    return {};
}

auto SSTableBuilder::CreateSSTable() -> Status {
    if (!dataBlockBuilder->Empty()) {
        dataBlockBuilder->CreateDataBlock();
        fileWriter->Append(dataBlockBuilder->Data().c_str(), dataBlockBuilder->Size(), true);

        std::string shortest_segment_key = last_key;
        shortest_segment_key.back() += 1;

        uint32_t block_size = dataBlockBuilder->Size();
        uint32_t block_offset = filterBlockInfo.size;
        filterBlockInfo.offset += dataBlockBuilder->Size();
        indexBlockBuilder->Add(shortest_segment_key, block_offset, block_size);
    
        indexBlock.append(indexBlockBuilder->Data());
        indexBlockBuilder->Clear();

        dataBlockBuilder->Clear();
    }

    filterBlockBuilder->CreateFilterBlock();
    filterBlockInfo.size = filterBlockBuilder->Size();
    fileWriter->Append(filterBlockBuilder->Data().c_str(), filterBlockBuilder->Size(), true);

    indexBlockInfo.offset = filterBlockInfo.offset + filterBlockInfo.size;
    indexBlockInfo.size = static_cast<uint32_t>(indexBlock.length());

    fileWriter->Append(indexBlock.c_str(), indexBlockInfo.size, true);


    footerBuilder->Add(filterBlockInfo.offset, filterBlockInfo.size,
                    indexBlockInfo.offset, indexBlockInfo.size);
    fileWriter->Append(footerBuilder->Data().c_str(), footerBuilder->Size(), true);

    dataBlockBuilder->Clear();
    indexBlockBuilder->Clear();
    footerBuilder->Clear();
    filterBlockBuilder = std::make_shared<FilterBlockBuilder>(keysNum, false_positive);

    filterBlockInfo.Clear();
    indexBlockInfo.Clear();
    indexBlock.clear();
    last_key.clear();
    return {};
}

auto SSTableBuilder::FindShortestSegmentKey(const std::string &key1, const std::string &key2) -> std::string {
    uint32_t size = static_cast<uint32_t>(std::min(key1.length(), key2.length()));
    uint32_t shared_length = 0;
    while (shared_length < size) {
        if (key1[shared_length] != key2[shared_length]) {
            break;
        }
        ++shared_length;
    }

    std::string shortest_segment_key;
    if (shared_length == key1.length()) {
        shortest_segment_key = key1 + " ";
    } else {
        shortest_segment_key = key1.substr(0, shared_length);
        shortest_segment_key.push_back(key1[shared_length] + 1);
    }
    
    return shortest_segment_key;

}

}