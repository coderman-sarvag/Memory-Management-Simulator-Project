#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <string>

struct MemoryBlock {
    int start;
    int size;
    bool free;
    std::string owner; 
};

#endif
