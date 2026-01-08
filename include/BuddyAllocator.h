#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <vector>
#include <unordered_map>
#include <string>

struct BuddyBlock
{
    int start;        
    int size;          
    std::string owner; 
};

class BuddyAllocator
{
private:
    int totalMemory;
    int minBlockSize;
    int maxOrder;
    int allocSuccess;
    int allocFailure;
    int nextId = 1;
    
    std::vector<std::vector<int>> freeLists;

   
    std::unordered_map<std::string, std::vector<BuddyBlock>> allocated;

public:
    BuddyAllocator();

    void init(int memorySize, int minBlock);

    void dump() const;
    void stats() const;
    bool isAllocated(const std::string &id) const { return allocated.count(id) > 0; }
    void allocate( int size);
    void freeProc(const std::string &proc);
};

#endif
