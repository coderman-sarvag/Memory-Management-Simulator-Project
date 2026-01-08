# User Guide – Memory Management Simulator

## Starting the Simulator
```text
./memsim
```
Basic Commands
Initialize Memory
```text
Basic Commands
Initialize Memory
```
Allocation Strategy
```text
Allocation Strategy
```
Allocate Memory
```text
malloc <size>
```
Free Memory
```text
free <process>
```
Dump Memory Layout
```text
dump
```
Memory Statistics
```text
stats
```
---

## Cache Commands
```text
cache_reset
cache_init <block> <L1> <L2> <L3>
cache_assoc <direct|set> [ways]
cache_stats
```

---

## Virtual Memory Commands
```text
vm_mode <fifo|lru>
vm_stats
access <process> <virtual_address>
```

---

## Buddy Allocation
```text
mode buddy
init <power_of_two>
malloc <process> <size>
```
Back to normal mode using
```text
mode normal
```

---

## Exiting the simulation
```text
exit
```