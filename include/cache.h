#ifndef  CACHE_H
#define CACHE_H
#include <stdbool.h>
#include "memory.h"


typedef struct {
    bool validBit;
    unsigned int tag;
    char block[32];
    unsigned int lastAccessTime;
} Line;

typedef struct {
    Line *cacheLines; //just one coz direct mapped
    int linesPerSet;
} Set;

typedef struct {
    Set* cacheSets;
    int numSets;
    int linesPerSet; //just easy to access lol
} Cache;


Cache* initalizeCache(Config *config);
void initializeSets(Set* sets, int numSets, int linesPerSet);
int getSetIndex(unsigned int addr, int numSets);
int getBlockOffset(unsigned int addr);
int getTagBits(unsigned int addr, int numSets);
int handleCacheAccess(Cache *cache, unsigned int addr, const char *data, const char policy[10]);
Line *randomReplacement(Set *set);
Line* leastRecentlyUsed(Set *set);
void updateCache(Line *line, int tagBits, const char *blockData);
void displayCache(Cache *c);
void freeCache(Cache *cache);

#endif


