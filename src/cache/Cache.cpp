#include "Cache.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

Cache::Cache(int bSize, int l1Size, int l2Size, int l3Size) {
    blockSize = bSize;
    l1Capacity = l1Size / blockSize;
    l2Capacity = l2Size / blockSize;
    l3Capacity = l3Size / blockSize;

    l1.resize(l1Capacity);
    l2.resize(l2Capacity);
    l3.resize(l3Capacity);

    assocType = SET_ASSOCIATIVE;
    ways = 2; 
    reset();
}
void Cache::setAssociativity(AssocType type, int w) {
    assocType = type;
    ways = max(1, w);   

    reset();

    if (assocType == DIRECT_MAPPED) {
        std::cout << "Cache associativity set to DIRECT-MAPPED (1-way)\n";
    } else {
        std::cout << "Cache associativity set to SET-ASSOCIATIVE ("
                  << ways << "-way)\n";
    }
}


void Cache::reset() {
   
    l1NumSets = max(1, l1Capacity / ways);
    l2NumSets = max(1, l2Capacity / ways);
    l3NumSets = max(1, l3Capacity / ways);

    auto clear = [](vector<CacheLine>& c) {
        for (auto &line : c) { line.valid = false; line.tag = -1; }
    };
    clear(l1); clear(l2); clear(l3);

    l1SetPointer.assign(l1NumSets, 0);
    l2SetPointer.assign(l2NumSets, 0);
    l3SetPointer.assign(l3NumSets, 0);

    l1Hits = l1Misses = l2Hits = l2Misses = l3Hits = l3Misses = 0;
}

void Cache::pushL(vector<CacheLine>& cache, vector<int>& pointers, int numSets, int setIdx, int tag) {
    int start = setIdx * ways;
    
    
    int victimIdx = start + pointers[setIdx];

  
    cache[victimIdx].valid = true;
    cache[victimIdx].tag = tag;
    pointers[setIdx] = (pointers[setIdx] + 1) % ways;
}
void Cache::access(int physicalAddress) {
    int blockNumber = physicalAddress / blockSize;

    int s1 = blockNumber % l1NumSets;
    for (int i = s1 * ways; i < (s1 * ways) + ways && i < (int)l1.size(); i++) {
        if (l1[i].valid && l1[i].tag == blockNumber) {
            l1Hits++; 
            cout << "L1 HIT" << endl; 
            return; 
        }
    }
    l1Misses++; 
    cout << "L1 MISS => Searching L2" << endl;
    int s2 = blockNumber % l2NumSets;
    bool foundInL2 = false;
    for (int i = s2 * ways; i < (s2 * ways) + ways && i < (int)l2.size(); i++) {
        if (l2[i].valid && l2[i].tag == blockNumber) {
            l2Hits++; 
            cout << "L2 HIT (Promoting to L1)" << endl; 
            foundInL2 = true; 
            break; 
        }
    }

    if (foundInL2) {
        
        pushL(l1, l1SetPointer, l1NumSets, s1, blockNumber);
        return;
    }

    l2Misses++; 
    cout << "L2 MISS => Searching L3" << endl;
    int s3 = blockNumber % l3NumSets;
    bool foundInL3 = false;
    for (int i = s3 * ways; i < (s3 * ways) + ways && i < (int)l3.size(); i++) {
        if (l3[i].valid && l3[i].tag == blockNumber) {
            l3Hits++; 
            cout << "L3 HIT (Promoting to L2 & L1)" << endl; 
            foundInL3 = true; 
            break;
        }
    }

    if (foundInL3) {
        pushL(l2, l2SetPointer, l2NumSets, s2, blockNumber);
        pushL(l1, l1SetPointer, l1NumSets, s1, blockNumber);
        return;
    }
    l3Misses++; 
    cout << "L3 MISS => Fetching from Physical Memory" << endl;
    pushL(l3, l3SetPointer, l3NumSets, s3, blockNumber);
    pushL(l2, l2SetPointer, l2NumSets, s2, blockNumber);
    pushL(l1, l1SetPointer, l1NumSets, s1, blockNumber);
}
void Cache::stats() const {
    cout << "\nCACHE STATS" << endl;
    cout << "L1 Hits: " << l1Hits << " | Misses: " << l1Misses << endl;
    cout << "L2 Hits: " << l2Hits << " | Misses: " << l2Misses << endl;
    cout << "L3 Hits: " << l3Hits << " | Misses: " << l3Misses << endl;

    auto calcRatio = [](int h, int m) {
        return (h + m == 0) ? 0.0 : (h * 100.0 / (h + m));
    };

    cout << fixed << setprecision(2);
    cout << "L1 Hit Ratio: " << calcRatio(l1Hits, l1Misses) << "%" << endl;
    cout << "L2 Hit Ratio: " << calcRatio(l2Hits, l2Misses) << "%" << endl;
    cout << "L3 Hit Ratio: " << calcRatio(l3Hits, l3Misses) << "%" << endl;
}