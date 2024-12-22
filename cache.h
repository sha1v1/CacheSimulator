#ifndef  CACHE_H
#define CACHE_H
#include <stdbool.h>


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
Set* initializeSets(Set* sets, int numSets);
int getSetIndex(unsigned int addr, int numSets);
int getBlockOffset(unsigned int addr);
int getTagBits(unsigned int addr, int numSets);
void handleCacheMiss(Line *line, int tagbits);
int accessCache(Cache *cache, unsigned int addr, char *data);

#endif


