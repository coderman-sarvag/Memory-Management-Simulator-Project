#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <vector>
#include <string>
#include "MemoryBlock.h"

class MemoryAllocator
{
private:
    int totalMemory;
    int nextId = 1;
    std::vector<MemoryBlock> blocks;
    std::string strategy;
    int allocSuccess;
    int allocFailure;

public:
    MemoryAllocator();
    const std::vector<MemoryBlock>& getBlocks() const { return blocks; }
    void init(int size);
    void setStrategy(const std::string &strat);
    long long internalFragmentation;
    void allocate( int size);
    void freeProc(const std::string &proc);

    void dump() const;
    void stats() const;
};

#endif
