#include "BuddyAllocator.h"
#include <iostream>
#include <cmath>
#include <algorithm>
int h;
int requested=0;
static int nextPowerOfTwo(int n)
{
    int power = 1;
    while (power < n)
        power <<= 1;
    return power;
}

using namespace std;

BuddyAllocator::BuddyAllocator()
{
    totalMemory = 0;
    minBlockSize = 0;
    maxOrder = 0;
}
void BuddyAllocator::init(int memorySize, int minBlock)
{
    totalMemory = memorySize;
    minBlockSize = minBlock;
    nextId =1;
    maxOrder = 0;
    int size = minBlockSize;
    while (size < totalMemory)
    {
        size *= 2;
        maxOrder++;
    }

    freeLists.clear();
    freeLists.resize(maxOrder + 1);

    allocated.clear();
    allocSuccess = 0;
    allocFailure = 0;

   
    freeLists[maxOrder].push_back(0);

    cout << "Buddy allocator initialized with memory "
         << totalMemory << " and min block "
         << minBlockSize << "\n";
}
void BuddyAllocator::allocate( int size)
{
    requested+=size;
    int roundedSize = nextPowerOfTwo(size);

    if (roundedSize < minBlockSize)
        roundedSize = minBlockSize;

    if (roundedSize > totalMemory)
    {
        allocFailure++;
        cout << "[BUDDY] Allocation failed: size too large\n";
        return;
    }

 
    int order = 0;
    int temp = roundedSize / minBlockSize;
    while (temp > 1)
    {
        temp >>= 1;
        order++;
    }

    int currentOrder = order;

    
    while (currentOrder <= maxOrder && freeLists[currentOrder].empty())
    {
        currentOrder++;
    }

    if (currentOrder > maxOrder)
    {
        allocFailure++;
        cout << "[BUDDY] Allocation failed: no suitable block\n";
        return;
    }

    int blockStart = freeLists[currentOrder].back();
    freeLists[currentOrder].pop_back();

    while (currentOrder > order)
    {
        currentOrder--;

        int buddyStart = blockStart + (minBlockSize << currentOrder);

        freeLists[currentOrder].push_back(buddyStart);
    }
    string proc = to_string(nextId++);
   
    BuddyBlock block;
    block.start = blockStart;
    block.size = roundedSize;
    block.owner = proc;

    allocated[proc].push_back(block);
    allocSuccess++;
    cout << "[BUDDY] Allocated " << roundedSize
         << " units to process: " << proc
         << " at address " << blockStart
         << " (requested " << size << ")\n";
}

void BuddyAllocator::freeProc(const std::string &proc)
{
    
    if (allocated.find(proc) == allocated.end())
    {
        cout << "[BUDDY] No memory allocated to process " << proc << "\n";
        return;
    }

    
    for (auto block : allocated[proc])
    {
        int blockStart = block.start;
        int blockSize = block.size;

        
        int order = 0;
        int temp = blockSize / minBlockSize;
        while (temp > 1)
        {
            temp >>= 1;
            order++;
        }

        
        while (order < maxOrder)
        {
            int buddyStart = blockStart ^ blockSize;

           
            auto &freeList = freeLists[order];
            auto it = std::find(freeList.begin(), freeList.end(), buddyStart);

            if (it == freeList.end())
            {
                
                break;
            }
            freeList.erase(it);
            blockStart = std::min(blockStart, buddyStart);
            blockSize *= 2;
            order++;
        }
        freeLists[order].push_back(blockStart);
    }
    allocated.erase(proc);

    cout << "[BUDDY] Freed memory of process " << proc << "\n";
}

void BuddyAllocator::dump() const
{
    cout << "\n[BUDDY] Memory Dump\n";

    for (int order = 0; order <= maxOrder; order++)
    {
        int blockSize = minBlockSize << order;
        cout << "Free blocks of size " << blockSize << ": ";

        if (freeLists[order].empty())
        {
            cout << "None";
        }
        else
        {
            for (int addr : freeLists[order])
            {
                cout << "[" << addr << "] ";
            }
        }
        cout << "\n";
    }


    cout << "\nAllocated blocks:\n";
    if (allocated.empty())
    {
        cout << "None\n";
    }
    else
    {
        for (const auto &entry : allocated)
        {
            const string &proc = entry.first;
            for (const auto &block : entry.second)
            {
                cout << "Process " << proc
                     << " -> [" << block.start
                     << ", size " << block.size << "]\n";
            }
        }
    }

    cout << "\n";
}
void BuddyAllocator::stats() const
{
    
    int reserved = 0;

    for (const auto &entry : allocated)
    {
        for (const auto &block : entry.second)
        {
            
            reserved += block.size;  
        }
    }

    int freeMemory = totalMemory - reserved;
    int internalFrag = reserved - requested;

    double utilization = 0.0;
    if (totalMemory > 0)
        utilization = (double)requested / reserved * 100.0;

    double internalFragPercent = 0.0;
    if (reserved > 0)
        internalFragPercent = (double)internalFrag / reserved * 100.0;

    cout << "\n[BUDDY] Memory Statistics\n";
    cout << "Total memory: " << totalMemory << "\n";
    cout << "Reserved memory: " << reserved << "\n";
    cout << "Requested memory: " << requested << "\n";
    cout << "Free memory: " << freeMemory << "\n";
    cout << "Internal fragmentation: " << internalFrag << "\n";
    cout << "Internal fragmentation (%): " << internalFragPercent << "%\n";
    cout << "Utilisation: " << utilization << "%\n\n";
    int total = allocSuccess + allocFailure;

    cout << "Allocation Successes: " << allocSuccess << "\n";
    cout << "Allocation Failures: " << allocFailure << "\n";

    if (total > 0)
    {
        cout << "Allocation Success Rate: "
             << (allocSuccess * 100.0 / total) << "%\n";
    }
}
