#include "cache.h"
#include <stdio.h>
#include <math.h>

void initializeSets(Set* sets, int numSets){
    int i;
    for(i = 0; i < numSets; i++){
        sets[i].cacheLine.validBit = 0;
        sets[i].cacheLine.tag = 0;
    }

    return sets;
}


Cache* initalizeCache(int numSets){
    
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    if(!cache){
        printf("Failed to allocate memory for Cache");
        exit();
    }
    cache->numSets = numSets;

    Set* sets = (Set*)malloc(numSets * sizeof(Set));
    if (!sets){
        printf("Failed to allocate memory for sets");
        exit();
    }

    cache->cacheSets = sets;

    initializeSets(cache->cacheSets, numSets);

    return cache;


}

int getSetIndex(unsigned int addr, int numSets){
    //right shift 5 bits to get rid of block offset bits
    //Then isolate set bits.
    return (addr >> 5) & (numSets - 1);
}


int getBlockOffset(unsigned int addr){
    //32 bytes/line => 5 bits to represent
    return addr & 31;
}

int getTagBits(unsigned int addr, int numSets){
    int setBits = (int)log2(numSets); //number of set bits
    return addr >> (5 + setBits);
}

int accessCache(Cache *cache, unsigned int addr){
    // set index, block offset, and tag bits
    int setIndex = getSetIndex(addr, cache->numSets);
    int blockOffset = getBlockOffset(addr);
    int tagBits = getTagBits(addr, cache->numSets);

    //get the line at the corresponding set index
    Line *line = &(cache->cacheSets[setIndex].cacheLine);

    //check valid bit and tagbits
    if(line->tag == tagBits && line->validBit){
        printf("Cache hit at set %d, block offset %d\n", setIndex, blockOffset);
        return 1;
    }
    else{
        printf("Cache miss at set %d\n", setIndex);
        return 0;
    }
}

