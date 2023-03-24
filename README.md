# MicroKV

MicroKV is a tiny kv engine basd on LSM architecture.

## build

The lib is built using cmake. You can run run.sh to build this repo on Linux.

```
chmod +x ./run.sh
./run.sh
```

## feature

* [x] Allocator
* [x] Memory Pool
* [x] MemTable
* [x] SkipList
* [x] Cache
* [x] SSTable
* [x] Bloom Filter
* [x] FileWriter / FileReader

## architecture

The storage system can be divided into following parts from top to down:

### Cache

When searching for a value, it will find in cache first. The cache has multiple partitions, each of which can set different cache policies. Now it support LRU policy.

### MemTable

If not found in cache, it will search in MemTable. MemTable is based on SkipList. So the time complexity of query, insert and delete operation is $O(logn)$.

### Allocator

The allocator is optimized for small memory allocation. It will request a block of memory in advance and organize it as blocks of different size. When allocating, it will chose a minimum block that can hold and allocate. For more detail, see

### SSTable

The data will be stored in SSTable format on disk, which is friendly for searching. The detailed SSTable format is in: 

## use

TODO

