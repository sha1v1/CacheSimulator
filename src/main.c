#include <stdio.h>
#include <stdlib.h>
#include "../include/cache.h"
#include "../include/config.h"

int main() {
    Config config;
    Cache *cache;

    // Read configuration
    printf("Reading configuration...\n");
    readConfigFile(&config);
    printf("Configuration loaded:\nnumber of sets = %d\nlines per set = %d\nmain memory size = %d bytes\nreplacement policy = %s\nwrite policy = %s", config.numSets, config.linesPerSet, config.mainMemorySize, config.replacementPolicy, config.writePolicy);

    // Initialize the cache
    cache = initalizeCache(config.numSets);
    printf("Cache initialized with %d sets.\n", config.numSets);

    // Interactive input
    unsigned int address;
    printf("\nEnter memory addresses (hex, e.g., 0x1234). Type 0x0 to quit.\n");

    while (1) {
        printf("Enter address: ");
        scanf("%x", &address);

        if (address == 0x0) {  // Exit condition
            break;
        }

        int hit = handleCacheAccess(cache, address, NULL, config.replacementPolicy);
        printf("Address 0x%X: %s\n", address, hit ? "Hit" : "Miss");
    }
    displayCache(cache);
    // Cleanup
    free(cache->cacheSets);
    free(cache);
    printf("Cache simulator exiting.\n");

    return 0;
}
