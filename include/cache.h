#ifndef  CACHE_H
#define CACHE_H
#include <stdbool.h>
#include "memory.h"


typedef struct {
    bool validBit;
    unsigned int tag;
    char block[32];
    
} Line;

typedef struct {
    Line cacheLine; //just one coz direct mapped
} Set;

typedef struct {
    Set* cacheSets;
    int numSets;
} Cache;


Cache* initalizeCache(int numSets);
void initializeSets(Set* sets, int numSets);
int getSetIndex(unsigned int addr, int numSets);
int getBlockOffset(unsigned int addr);
int getTagBits(unsigned int addr, int numSets);
void handleCacheMiss(Memory *memory, int addr, Line *line, int tagbits);
int accessCache(Cache *cache, Memory *memory,unsigned int addr);
void displayCache(Cache *c);

#endif


