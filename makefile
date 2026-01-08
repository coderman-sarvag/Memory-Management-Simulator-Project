CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = src/main.cpp \
      src/allocator/MemoryAllocator.cpp \
      src/buddy/BuddyAllocator.cpp \
      src/cache/Cache.cpp \
      src/vm/VirtualMemory.cpp

OUT = memsim

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o src/$(OUT)

clean:
	rm -f src/$(OUT) src/$(OUT).exe