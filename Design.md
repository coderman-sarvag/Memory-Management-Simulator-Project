
---

# 📄 2. `DESIGN.md`

```md
# Design Document – Memory Management Simulator

## 1. Overview
This simulator models key OS memory subsystems in a modular and extensible manner:
- Memory allocation
- Cache hierarchy
- Virtual memory

Each subsystem is independent but integrated through a unified CLI.

---

## 2. Memory Allocation Design

### 2.1 Contiguous Allocation
- Memory is represented as a list of variable-sized blocks.
- Allocation strategies:
  - First Fit
  - Best Fit
  - Worst Fit
- Free blocks are merged to reduce external fragmentation.

### 2.2 Fragmentation Metrics
- **External Fragmentation**: fragmented free memory blocks
- **Internal Fragmentation**: unused space inside allocated blocks (mainly in buddy system)
- **Utilization**: Utilization = Requested Memory / Reserved Memory

---

## 3. Buddy Allocator Design
- Memory size must be a power of two.
- Allocation rounds requests to the nearest power of two.
- Blocks are split recursively.
- Free blocks are merged with their buddies.
- Internal fragmentation is implicit due to rounding.

---

## 4. Cache Design

### 4.1 Architecture
- Three-level cache: L1, L2, L3
- Configurable:
- Block size
- Cache sizes
- Associativity

### 4.2 Mapping
- Direct-mapped or set-associative
- FIFO replacement within each set

### 4.3 Cache Access Flow
1. Check L1
2. On miss → check L2
3. On miss → check L3
4. On miss → fetch from memory and fill upward

### 4.4 Statistics
- Hits and misses per cache level
- Hit ratios

---

## 5. Virtual Memory Design

### 5.1 Address Translation
- Virtual address = page number + offset
- Physical address = frame number × page size + offset

### 5.2 Page Replacement Policies
- FIFO
- LRU (Least Recently Used)

### 5.3 Frame Reuse
- When no free frames exist, an evicted page’s frame is reused.
- Frame reuse is explicitly tracked.

### 5.4 Disk Access
- Disk access latency is simulated symbolically on page faults.

---

## 6. Integration Flow

User Command
↓
Memory Allocation (process exists)
↓
Virtual Memory Translation
↓
Cache Lookup (L1 → L2 → L3)
↓
Main Memory

---

## 7. Design Choices
- Utilization depends on reserved memory, not requested memory.
- Cache uses FIFO only for simplicity.
- Modular class-based design for clarity and extensibility.

---

## 8. Conclusion
The simulator faithfully represents OS memory behavior while remaining simple and demonstrable.

