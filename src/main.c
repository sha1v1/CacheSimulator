#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cache.h"
#include "../include/memory.h"
#include "../include/config.h"

Config *config;
Memory *memory;
Cache *cache;

/**
 * @brief an initial booting up of sorts to run the application
 */
void init(){
    //read the config file 
    // Allocate memory for config, memory, and cache
    config = malloc(sizeof(Config));
    if (!config) {
        printf("Error: Failed to allocate memory for Config.\n");
        exit(1);
    }

    memory = malloc(sizeof(Memory));
    if (!memory) {
        printf("Error: Failed to allocate memory for Memory.\n");
        free(config);
        exit(1);
    }
    readConfigFile(config);
    cache = initalizeCache(config);
    initializeMemory(memory, config);


}

//this is only supposed to handle a read operation. In case of a hit, its quite simple.
//In case of a cold miss, the cache line is loaded with the data  blcok fetched from memory.
//In case of a conflict miss, suitable line based on the eviction policy is removed followed by fetching of the data block from the memory.
char handleRead(Cache *cache, Memory *memory, unsigned int addr, const char policy[10]){
    //user enters an address
    //look for this in the cache first
    //if its a hit
        //return the value and exit the method
    //if its a miss
        // read that address from memory
        // evict a line from cache based on replacement policy, 
        // fetch that block from memory to load it in the cache
        //exit method
    //if its a conflict miss
        //read from memory again
        // fetch block from memory, evict a line from cache based on replacement policy, load into cache
        // exit method
    
    //look in cache
    char fetchedData;
    int hit;
    hit = checkCache(cache, addr, &fetchedData);
            printf("here");

    switch(hit){
        case 1:
            return fetchedData; //successfull cache access, didnt have to bother looking into memory
        
        case 0:
            printf("Cache miss! Fetching data from memory...\n");
            char *blockData = fetchBlockFromMemory(memory, addr); // Fetch block from memory
            fetchedData = (char)readFromMemory(memory, addr);
            printf("here1\n");
            if (!blockData) {
                printf("Error: Failed to fetch block from memory.\n");
                return -1;
            }

            // Handle cache replacement for the fetched block
            Line *lineToReplace = handleLineReplacement(cache, addr, policy);
            printf("%p\n", lineToReplace);
            printf("here4\n");
            if (!lineToReplace) {
                printf("Error: Cache replacement failed.\n");
                free(blockData); // Free the block data fetched from memory
                return 1;
            }

            // Update the cache line with the fetched block
            int tagBits = getTagBits(addr, cache->numSets);
            printf("here5\n");
            updateCache(lineToReplace, tagBits, blockData);
            // Get the data at the specific block offset within the fetched block
            int blockOffset = getBlockOffset(addr);
            printf("Block offset: %d\n", blockOffset);
            printf("Data at address 0x%X (loaded from memory): %d\n", addr, blockData[blockOffset]);

            free(blockData); // Free the block data
            return fetchedData;

        default: // Error state
            printf("Error: Invalid cache access.\n");
            return -1;

    }
}

int main(){
    printf("here2\n");
    init();
        printf("here3\n");

    handleRead(cache, memory, 0x100, "RANDOM");
        printf("here2\n");

    displayCache(cache);
    return 0;
}