#ifndef CACHE_H
#define CACHE_H

#include <vector>

struct CacheLine
{
    bool valid;
    int tag;
};

enum AssocType
{
    DIRECT_MAPPED,
    SET_ASSOCIATIVE
};

class Cache
{
private:
    
    AssocType assocType;
    int ways;
    int blockSize;

    int l1Capacity, l2Capacity, l3Capacity;
    int l1NumSets, l2NumSets, l3NumSets;

    std::vector<CacheLine> l1, l2, l3;
    std::vector<int> l1SetPointer, l2SetPointer, l3SetPointer;

    int l1Hits, l1Misses, l2Hits, l2Misses, l3Hits, l3Misses;

    void pushL(std::vector<CacheLine> &cache, std::vector<int> &pointers, int numSets, int setIdx, int tag);

public:
    Cache(int blockSize = 16, int l1Size = 64, int l2Size = 256, int l3Size = 1024);
    void access(int physicalAddress);
    void setAssociativity(AssocType type, int w);
    void stats() const;
    void reset();
};

#endif