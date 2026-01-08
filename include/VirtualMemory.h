#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <unordered_map>
#include <queue>
#include <string>
#include <climits>

class VirtualMemory
{
private:
    int pageSize;
    int totalFrames;
    std::queue<int> freeFrames;
    std::queue<std::pair<std::string, int>> fifoQueue;

    
    std::unordered_map<
        std::string,
        std::unordered_map<int, int>
    > pageTable;
    std::unordered_map<
        std::string,
        std::unordered_map<int, int>
    > lastUsed;

    int globalTime;


    int pageFaults;
    int pageHits;
    int pageReplacements;


    enum VMPolicy { FIFO_VM, LRU_VM };
    VMPolicy policy;

public:
    VirtualMemory(int memSize = 0, int pSize = 64);
    int frameReuses;
    void init(int memSize);
    int access(const std::string &proc, int virtualAddr);
    int diskAccesses;
    void stats() const;
    void reset();

    void setPolicyFIFO();
    void setPolicyLRU();
};

#endif
