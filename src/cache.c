#include "../include/cache.h"
#include "../include/memory.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void initializeSets(Set *sets, int numSets, int linesPerSet){
    
    // outer loop initializes each set
    for(int i = 0; i < numSets; i++){
        sets[i].linesPerSet = linesPerSet;
        sets[i].cacheLines = (Line *)malloc(linesPerSet * sizeof(Line));
        if(!sets[i].cacheLines){
            printf("Failed to allocate memory for lines in set %d", i);
            exit(1);
        }
        
        //now initialize each line within a set
        for (int j = 0; j < linesPerSet; j++){
            sets[i].cacheLines[j].validBit = false;
            sets[i].cacheLines[j].tag = 0;
            memset(sets[i].cacheLines[j].block, 0, sizeof(sets[i].cacheLines[j].block));
        }

    }

}


Cache* initalizeCache(Config *config){
    if(!config){
        printf("Error: Configurations have not been set.");
        return NULL;
    }

    Cache* cache = (Cache*)malloc(sizeof(Cache));
    if(!cache){
        printf("Failed to allocate memory for Cache");
        exit(1);
    }
    cache->numSets = config->numSets;
    cache->linesPerSet = config->linesPerSet;

    Set* sets = (Set*)malloc(cache->numSets * sizeof(Set));
    if (!sets){
        printf("Failed to allocate memory for sets");
        exit(1);
    }

    cache->cacheSets = sets;

    //initialize all sets
    initializeSets(cache->cacheSets, cache->numSets, cache->linesPerSet);

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
 * In case of a cache miss, fetch the line of data from memory 
 * and populate the said block
 */
// void handleCacheMiss(int addr, Line *line, int tagbits){
//     line->validBit = true;
//     line->tag = tagbits;

//     int blockStartAddress = addr & ~31;
//     for (int i = 0; i < 32; i++) {       // 32 bytes per block
//         line->block[i] = readFromMemory(memory, blockStartAddress + i);
//     }
//     printf("Loaded block from main memory to cache\n");

// }

int accessCache(Cache *cache, unsigned int addr, const char *data){
    //check if pointer is valid
    if (!cache || !cache->cacheSets) {
        printf("Error: Attempting to access an uninitialized cache.\n");
        return -1;  
    }

    // set index, block offset, and tag bits
    int setIndex = getSetIndex(addr, cache->numSets);
    int blockOffset = getBlockOffset(addr);
    int tagBits = getTagBits(addr, cache->numSets);

    Set *curSet = &(cache->cacheSets[setIndex]);

    //get the lines at the corresponding set index
    Line *lines = curSet->cacheLines;

    //iterate through all lines
    for(int i = 0; i < curSet->linesPerSet; i++){
        //check valid bit and tagbits
        if(lines[i].tag == tagBits && lines[i].validBit){
        printf("Cache hit at set %d, block offset %d\n", setIndex, blockOffset);
        return 1; //cache hit
        }
    }

    printf("Cache miss at set %d\n", setIndex); 

    //now look for emptyline or a line to replace
    Line *lineToReplace = NULL;
    for (int i = 0; i < curSet->linesPerSet; i++) {
        if (!lines[i].validBit) {
            lineToReplace = &lines[i];   // Found an empty line
            break;
        }
    }

    //if we don't find one, replace one
    if(lineToReplace == NULL){
        lineToReplace = randomReplacement(curSet);
    }

    //update the chosen line
    updateCache(lineToReplace, tagBits, data);

    return 0;
    

}

Line *randomReplacement(Set *set){
    // this runs if an empty line wasn't found
    int lineToReplaceIndex = rand() % set->linesPerSet;
    return &set->cacheLines[lineToReplaceIndex];

}


void updateCache(Line *line, int tagBits, const char *blockData) {
    // Line *line = &(cache->cacheSets[setIndex].cacheLines);
    line->validBit = true;
    line->tag = tagBits;
    strcpy(line->block, blockData);  // Copy data into cache block
}

void freeCache(Cache *cache) {
    if (!cache) return;  // Ensure the cache pointer is valid

    // Free the array of sets
    for (int i = 0; i < cache->numSets; i++) {
        Set *set = &(cache->cacheSets[i]);
        if (set->cacheLines) {
            free(set->cacheLines);
            set->cacheLines = NULL;
        }
    }

    free(cache->cacheSets);
    free(cache);
}

/**
 * Display the current state of the cache.
 * Format:
 * Set <set number>: <valid bit>  <tag>  <block data>
 */
void displayCache(Cache *cache) {
    if (!cache || !cache->cacheSets) {
        printf("Error: Cache is not initialized.\n");
        return;
    }

    int numSets = cache->numSets;
    int linesPerSet = cache->linesPerSet;

    printf("\n\n*****CACHE STATE*****\n");
    printf("Set | Line | Valid | Tag     | Block Data\n");
    printf("-----------------------------------------\n");

    for (int i = 0; i < numSets; i++) {
        Set *set = &cache->cacheSets[i];
        for (int j = 0; j < linesPerSet; j++) {
            Line *line = &set->cacheLines[j];
            printf("%3d | %4d | %5d | %7u | %s\n", 
                   i, j, line->validBit, line->tag, line->block);
        }
    }
    printf("-----------------------------------------\n");
}

