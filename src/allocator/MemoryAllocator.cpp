#include "MemoryAllocator.h"
#include <iostream>

using namespace std;

MemoryAllocator::MemoryAllocator()
{
    totalMemory = 0;
    strategy = "first";
    internalFragmentation = 0;
}

void MemoryAllocator::init(int size)
{
    blocks.clear();
    totalMemory = size;
    internalFragmentation = 0;
    MemoryBlock block;
    block.start = 0;
    block.size = size;
    block.free = true;
    block.owner = "";
    nextId =1;
    blocks.push_back(block);
    allocSuccess = 0;
    allocFailure = 0;

    cout << "Memory initialized with size " << size << "\n";
}
void MemoryAllocator::setStrategy(const string &strat)
{
    if (strat == "first" || strat == "best" || strat == "worst")
    {
        strategy = strat;
        cout << "Allocation strategy set to " << strategy << "\n";
    }
    else
    {
        cout << "Invalid strategy. Use first, best, or worst.\n";
    }
}
void MemoryAllocator::allocate(int size)
{
    int chosenIndex = -1;

    for (size_t i = 0; i < blocks.size(); i++)
    {

        if (!blocks[i].free)
            continue;
        if (blocks[i].size < size)
            continue;

        if (strategy == "first")
        {
            chosenIndex = i;
            break;
        }

        if (strategy == "best")
        {
            if (chosenIndex == -1 ||
                blocks[i].size < blocks[chosenIndex].size)
            {
                chosenIndex = i;
            }
        }

        if (strategy == "worst")
        {
            if (chosenIndex == -1 ||
                blocks[i].size > blocks[chosenIndex].size)
            {
                chosenIndex = i;
            }
        }
    }
    string proc = to_string(nextId++);

    if (chosenIndex == -1)
    {
        allocFailure++;
        cout << "Allocation failed for process " << proc
             << ": not enough contiguous memory\n";

        return;
    }

    MemoryBlock &block = blocks[chosenIndex];

    if (block.size == size)
    {
        block.free = false;
        block.owner = proc;
    }
    else
    {
        MemoryBlock usedBlock;
        usedBlock.start = block.start;
        usedBlock.size = size;
        usedBlock.free = false;
        usedBlock.owner = proc;

        MemoryBlock freeBlock;
        freeBlock.start = block.start + size;
        freeBlock.size = block.size - size;
        freeBlock.free = true;
        freeBlock.owner = "";

        blocks[chosenIndex] = usedBlock;
        blocks.insert(blocks.begin() + chosenIndex + 1, freeBlock);
    }
    allocSuccess++;
    internalFragmentation += (block.size - size);
    cout << "Allocated " << size << " units to process: " << proc
         << " at address " << blocks[chosenIndex].start << "\n";
}

void MemoryAllocator::freeProc(const string &proc)
{
    bool found = false;

    for (auto &block : blocks)
    {
        if (!block.free && block.owner == proc)
        {
            block.free = true;
            block.owner = "";
            found = true;
        }
    }

    if (!found)
    {
        cout << "No memory found for process " << proc << "\n";
        return;
    }

    for (size_t i = 0; i + 1 < blocks.size();)
    {

        if (blocks[i].free && blocks[i + 1].free)
        {

            blocks[i].size += blocks[i + 1].size;
            blocks.erase(blocks.begin() + i + 1);
        }
        else
        {
            i++;
        }
    }

    cout << "Freed memory of process " << proc << "\n";
}

void MemoryAllocator::dump() const
{
    cout << "Memory Layout:\n";

    for (const auto &block : blocks)
    {
        int end = block.start + block.size - 1;

        cout << "[" << block.start << " - " << end << "] ";

        if (block.free)
        {
            cout << "FREE\n";
        }
        else
        {
            cout << "USED (" << block.owner << ")\n";
        }
    }
}
void MemoryAllocator::stats() const
{
    int used = 0;
    int free = 0;
    int largestFree = 0;

    for (const auto &block : blocks)
    {
        if (block.free)
        {
            free += block.size;
            if (block.size > largestFree)
            {
                largestFree = block.size;
            }
        }
        else
        {
            used += block.size;
        }
    }

    cout << "Memory Statistics:\n";
    cout << "Total memory: " << totalMemory << "\n";
    cout << "Used memory: " << used << "\n";
    cout << "Free memory: " << free << "\n";
    cout << "Largest free block: " << largestFree << "\n";

    double fragmentation = 0.0;
    if (free > 0)
    {
        fragmentation = (1.0 - (double)largestFree / free) * 100.0;
    }

    double utilization = (double)used / totalMemory * 100.0;
    int total = allocSuccess + allocFailure;

    cout << "Allocation Successes: " << allocSuccess << "\n";
    cout << "Allocation Failures: " << allocFailure << "\n";

    if (total > 0)
    {
        cout << "Allocation Success Rate: "
             << (allocSuccess * 100.0 / total) << "%\n";
    }

    cout << "External fragmentation: " << fragmentation << "%\n";
    cout << "Internal Fragmentation: "
         << internalFragmentation << " units\n";
    cout << "Memory utilization: " << utilization << "%\n";
}
