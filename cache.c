#include "cache.h"
#include <stdio.h>

Set* initializeSets(Set* sets, int numSets){
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


}