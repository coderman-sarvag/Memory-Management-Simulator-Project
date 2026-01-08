#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "MemoryAllocator.h"
#include "BuddyAllocator.h"
#include "Cache.h"
#include "VirtualMemory.h"
using namespace std;
MemoryAllocator memAllocator;
BuddyAllocator buddyAllocator;
VirtualMemory vm;
vector<string> tokenize(const string &line)
{
    vector<string> tokens;
    string word;
    stringstream ss(line);
    while (ss >> word)
    {
        tokens.push_back(word);
    }
    return tokens;
}
enum AllocatorMode
{
    NORMAL,
    BUDDY
};
AllocatorMode currentMode = NORMAL;
bool isPowerOfTwo(int n)
{
    if (n <= 0)
        return false;
    return (n & (n - 1)) == 0;
}
int lastInitSize = 0;
int cacheBlockSize = 64;
int l1Size = 256;
int l2Size = 512;
int l3Size = 1024;
Cache cache(cacheBlockSize, l1Size, l2Size, l3Size);

int main()
{

    cout << "Memory Simulator Started\n";
    cout << "Give input as help for getting the list of commands\n";
    string line;

    while (true)
    {
        cout << "> ";
        getline(cin, line);

        if (line.empty())
            continue;

        vector<string> tokens = tokenize(line);
        string cmd = tokens[0];

        if (cmd == "exit")
        {
            cout << "exiting...";
            break;
        }
        else if (cmd == "help")
        {
            cout << "Mode stays normal if mode not set.\n";
            cout << "Firstly command init to be called to declare memory then mode\n ";
            cout << "Commands:\n";
            cout << "mode <normal|buddy>\n";
            cout << "init <size> \n";
            cout << "strategy <first|best|worst>\n";
            cout << "malloc <size>\n";
            cout << "free <proc>\n";
            cout << "access <proc> <addr>\n";
            cout << "cache_stats\n";
            cout << "cache_reset\n";
            cout << "cache_assoc\n";
            cout << "vm_stats\n";
            cout << "dump\n";
            cout << "stats\n";
            cout << "exit\n";
        }
        else if (cmd == "init")
        {

            if (tokens.size() != 2)
            {
                cout << "Usage: init <size>\n";
                continue;
            }

            int size = stoi(tokens[1]);
            memAllocator.init(size);
            buddyAllocator.init(size, 64);
            cache.reset();
            vm.init(size);
            lastInitSize = size;
            cout << "System initialized with size " << size << "\n";
        }
        else if (cmd == "mode")
        {
            if (tokens.size() != 2)
            {
                cout << "Usage: mode <normal|buddy>\n";
                continue;
            }
            if (tokens[1] == "normal")
            {
                currentMode = NORMAL;
                cout << "Switched to NORMAL allocator mode\n";
            }
            else if (tokens[1] == "buddy")
            {
                if (!isPowerOfTwo(lastInitSize))
                {
                    if (lastInitSize == 0)
                    {
                        cout << "[ERROR] Buddy allocator requires memory size to be a power of two.\n";
                        cout << "Current size: " << lastInitSize << "\n";
                        cout << "Initialise memory first.\n";
                        continue;
                    }
                    else
                    {
                        cout << "[ERROR] Buddy allocator requires memory size to be a power of two.\n";
                        cout << "Current size: " << lastInitSize << "\n";
                        cout << "Staying in Normal mode\n";
                        currentMode = NORMAL;
                        continue;
                    }
                }

                currentMode = BUDDY;
                cout << "Switched to BUDDY allocator mode\n";
            }
            else
            {
                cout << "Invalid mode. Use normal or buddy.\n";
            }
        }
        else if (cmd == "strategy")
        {
            if (currentMode == NORMAL)
            {
                if (tokens.size() != 2)
                {
                    cout << "Usage: strategy <first|best|worst>\n";
                    continue;
                }
                memAllocator.setStrategy(tokens[1]);
            }
            else
            {
                cout << "Strategy is not a valid command for mode:buddy.\n";
            }
        }
        else if (cmd == "malloc")
        {
            if (tokens.size() != 2)
            { 
                cout << "Usage: malloc <size>\n";
                continue;
            }
            int size = stoi(tokens[1]);
            if (currentMode == NORMAL)
                memAllocator.allocate(size);
            else
                buddyAllocator.allocate(size);
        }
        else if (cmd == "access")
        {
            if (tokens.size() != 3)
            {
                cout << "Usage: access <proc> <virtual_addr>\n";
                continue;
            }
            string targetId = tokens[1];
            int virtualAddr = stoi(tokens[2]);

            
            bool idExists = false;
            if (currentMode == NORMAL)
            {
                for (const auto &block : memAllocator.getBlocks())
                {
                    if (!block.free && block.owner == targetId)
                    {
                        idExists = true;
                        break;
                    }
                }
            }
            else
            {
                idExists = buddyAllocator.isAllocated(targetId);
            }

            if (!idExists)
            {
                cout << "[ERROR] Access Denied: Block ID " << targetId << " is not allocated. First allocate memory using malloc.\n";
                continue; 
            }
            string proc = tokens[1];
            

            cout << "[ACCESS] Process " << proc
                 << " accessing virtual address " << virtualAddr << "\n";

            
            int physicalAddr = vm.access(proc, virtualAddr);

            cout << "[ACCESS] Translated to physical address "
                 << physicalAddr << "\n";

            
            cache.access(physicalAddr);
        }
        else if (cmd == "vm_stats")
        {
            vm.stats();
        }
        else if (cmd == "cache_stats")
        {
            cache.stats();
        }
        else if (cmd == "cache_init")
        {
            if (tokens.size() != 5)
            {
                cout << "Usage: cache_init <block> <L1> <L2> <L3>\n";
                continue;
            }

            cacheBlockSize = stoi(tokens[1]);
            l1Size = stoi(tokens[2]);
            l2Size = stoi(tokens[3]);
            l3Size = stoi(tokens[4]);

            cache = Cache(cacheBlockSize, l1Size, l2Size, l3Size);
            cout << "Cache reconfigured\n";
        }
        else if (cmd == "cache_assoc")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: cache_assoc <direct|set> [ways]\n";
                continue;
            }

            if (tokens[1] == "direct")
            {
                cache.setAssociativity(DIRECT_MAPPED, 1);
            }
            else if (tokens[1] == "set" && tokens.size() == 3)
            {
                cache.setAssociativity(SET_ASSOCIATIVE, stoi(tokens[2]));
            }
        }

        else if (cmd == "cache_reset")
        {
            cache.reset();
            cout << "Cache reset\n";
        }
        else if (cmd == "dump")
        {
            if (currentMode == NORMAL)
            {
                memAllocator.dump();
            }
            else
            {
                buddyAllocator.dump();
            }
        }
        else if (cmd == "free")
        {
            if (tokens.size() != 2)
            {
                cout << "Usage: free <proc>\n";
                continue;
            }

            if (currentMode == NORMAL)
            {
                memAllocator.freeProc(tokens[1]);
            }
            else
            {
                buddyAllocator.freeProc(tokens[1]);
            }
        }
        else if (cmd == "stats")
        {
            if (currentMode == NORMAL)
            {
                memAllocator.stats();
            }
            else
            {
                buddyAllocator.stats();
            }
        }
        else if (cmd == "vm_mode")
        {
            if (tokens.size() != 2)
            {
                cout << "Usage: vm_mode <fifo|lru>\n";
                continue;
            }

            if (tokens[1] == "fifo")
            {
                vm.setPolicyFIFO();
            }
            else if (tokens[1] == "lru")
            {
                vm.setPolicyLRU();
            }
            else
            {
                cout << "Invalid VM mode. Use fifo or lru.\n";
            }
        }

        else
        {
            cout << "Unknown command\n";
        }
    }
    return 0;
}
