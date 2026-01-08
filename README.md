# Memory Management Simulator

A C++ command-line simulator demonstrating core **Operating System memory management concepts**, including:
- Contiguous memory allocation strategies
- Buddy memory allocation
- Multilevel cache hierarchy
- Virtual memory with paging and replacement policies

This project is designed for **educational purposes** and closely follows OS textbook models.

---

## ✨ Features

### Memory Allocation
- First Fit
- Best Fit
- Worst Fit
- Freeing and merging blocks
- External fragmentation tracking
- Memory utilization statistics

### Buddy Allocation
- Power-of-two block allocation
- Splitting and coalescing
- Internal fragmentation (conceptual)
- Separate mode from normal allocation

### Cache Simulation
- L1, L2, L3 cache hierarchy
- Configurable cache sizes and block size
- Direct-mapped and set-associative cache
- FIFO replacement policy
- Hit/miss statistics per cache level

### Virtual Memory
- Per-process page tables
- Virtual to physical address translation
- FIFO and LRU page replacement
- Page fault handling
- Frame reuse tracking
- Symbolic disk access simulation

---

## 🛠️ Build Instructions

### Using g++ (recommended via MSYS2 / Linux)

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o memsim
```
### Run
```bash
./memsim
```
## How to Use
Type help inside the simulator to see all available commands.
Example:
```bash
init 1024
strategy first
malloc A 100
dump
stats
```
## Project Structure
Memory-Management-Simulator/
│
├── include/                # Header files (.h)
│   ├── MemoryAllocator.h
│   ├── BuddyAllocator.h
│   ├── Cache.h
│   ├── VirtualMemory.h
│   └── MemoryBlock.h
│
├── src/                    # Source files (.cpp)
│   ├── main.cpp
│   ├── MemoryAllocator.cpp
│   ├── BuddyAllocator.cpp
│   ├── Cache.cpp
│   └── VirtualMemory.cpp
│
├── tests/                  # Test input files
│   ├── memory_test.txt
│   ├── cache_test.txt
│   ├── vm_test.txt
│   └── buddy_test.txt
│
├── README.md
├── DESIGN.md
├── USER_GUIDE.md
└── Makefile

---

## Demo Video
Demo video is accessible in the repository as well as on the google drive link : https://drive.google.com/drive/folders/1_beRswbxMwOQG_4_5vqkYgLAi2zT8E0j?usp=drive_link

---

## Notes
1. Cache and virtual memory access require an allocated process.
2. Cache is demonstrated before virtual memory in the demo.
3. All statistics are printed using stats, cache_stats, and vm_stats.

