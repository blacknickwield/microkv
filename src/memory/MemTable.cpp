#include "memory/MemTable.hpp"

namespace microkv {

MemTable::MemTable(std::shared_ptr<Allocator> allocator_) : allocator(allocator_) {

}

}