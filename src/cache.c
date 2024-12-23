#include "../include/cache.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void initializeSets(Set* sets, int numSets){
    int i;
    for(i = 0; i < numSets; i++){
        sets[i].cacheLine.validBit = false;
        sets[i].cacheLine.tag = 0;
    }

}


Cache* initalizeCache(int numSets){
    
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    if(!cache){
        printf("Failed to allocate memory for Cache");
        exit(1);
    }
    cache->numSets = numSets;

    Set* sets = (Set*)malloc(numSets * sizeof(Set));
    if (!sets){
        printf("Failed to allocate memory for sets");
        exit(1);
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

/**
 * In case of a cache miss, fill the entire block with placeholder string "WrittenData"
 * Writing a specific part of a block requires main memory to be simulated as well.
 */
void handleCacheMiss(Line *line, int tagbits){
    line->validBit = true;
    line->tag = tagbits;
    strcpy(line->block, "WrittenData");

}

int accessCache(Cache *cache, unsigned int addr, char *data){
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
        handleCacheMiss(line, tagBits);
        return 0;
    }
}

/**
 * Display the current state of the cache.
 * Format:
 * Set <set number>: <valid bit>  <tag>  <block data>
 */
void displayCache(Cache *c){
    int numSets = c->numSets;
    printf("\n\n*****CACHE*****\n");
    printf("       v  t   b\n");
    for(int i = 0; i < numSets; i++){
        Line l = c->cacheSets[i].cacheLine;
        printf("set %d: %d  %d  %s\n", i, l.validBit, l.tag, l.block);
    }
}

