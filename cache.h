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

#endif


