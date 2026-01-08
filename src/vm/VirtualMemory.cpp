#include "VirtualMemory.h"
#include <iostream>

using namespace std;

VirtualMemory::VirtualMemory(int memSize, int pSize)
{
    pageSize = pSize;
    policy = FIFO_VM;
    globalTime = 0;
    diskAccesses=0;
    reset();

    if (memSize > 0)
        init(memSize);
}

void VirtualMemory::reset()
{
    while (!freeFrames.empty())
        freeFrames.pop();
    while (!fifoQueue.empty())
        fifoQueue.pop();
    frameReuses = 0;
    pageTable.clear();
    lastUsed.clear();
    diskAccesses=0;
    pageFaults = 0;
    pageHits = 0;
    pageReplacements = 0;
    globalTime = 0;
}

void VirtualMemory::init(int memSize)
{
    reset();

    totalFrames = memSize / pageSize;

    for (int i = 0; i < totalFrames; i++)
        freeFrames.push(i);

    cout << "[VM] Initialized with " << totalFrames
         << " frames of size " << pageSize << "\n";
}

int VirtualMemory::access(const string &proc, int virtualAddr)
{
    int page = virtualAddr / pageSize;
    int offset = virtualAddr % pageSize;

    if (pageTable[proc].count(page))
    {
        pageHits++;
        lastUsed[proc][page] = globalTime++;
        int frame = pageTable[proc][page];
        return frame * pageSize + offset;
    }

    pageFaults++;
    diskAccesses++;
    cout << "[VM] Page fault for process "
         << proc << " on page " << page << "\n";

    int frame;

    if (!freeFrames.empty())
    {
        frame = freeFrames.front();
        freeFrames.pop();
    }
    else
    {
        pageReplacements++;

        if (policy == FIFO_VM)
        {
            auto victim = fifoQueue.front();
            fifoQueue.pop();

            string victimProc = victim.first;
            int victimPage = victim.second;

            frame = pageTable[victimProc][victimPage];
            pageTable[victimProc].erase(victimPage);
            lastUsed[victimProc].erase(victimPage);
            frameReuses++;
            cout << "[VM] Evicted page " << victimPage
                 << " of process " << victimProc << "\n";
            cout << "[VM] Reusing physical frame "
                 << frame << " for new page\n";
        }
        else
        {
            string victimProc;
            int victimPage = -1;
            int minTime = INT_MAX;

            for (auto &p : lastUsed)
            {
                for (auto &pg : p.second)
                {
                    if (pg.second < minTime)
                    {
                        minTime = pg.second;
                        victimProc = p.first;
                        victimPage = pg.first;
                    }
                }
            }

            frame = pageTable[victimProc][victimPage];
            pageTable[victimProc].erase(victimPage);
            lastUsed[victimProc].erase(victimPage);
            frameReuses++;
            cout << "[VM] Evicted page " << victimPage
                 << " of process " << victimProc << " (LRU)\n";
            cout << "[VM] Reusing physical frame "
                 << frame << " for new page\n";
        }
    }

    pageTable[proc][page] = frame;
    lastUsed[proc][page] = globalTime++;

    if (policy == FIFO_VM)
        fifoQueue.push({proc, page});

    return frame * pageSize + offset;
}

void VirtualMemory::stats() const
{
    cout << "\n[VM STATS]\n";
    cout << "Page Hits: " << pageHits << "\n";
    cout << "Page Faults: " << pageFaults << "\n";
    cout << "Page Replacements: " << pageReplacements << "\n\n";
    cout << "Frame Reuses: " << frameReuses << "\n";
    cout<<"Disk Accesses (simulated): "<<diskAccesses<<"\n\n";
}

void VirtualMemory::setPolicyFIFO()
{
    policy = FIFO_VM;
    cout << "[VM] Replacement policy set to FIFO\n";
}

void VirtualMemory::setPolicyLRU()
{
    policy = LRU_VM;
    cout << "[VM] Replacement policy set to LRU\n";
}
