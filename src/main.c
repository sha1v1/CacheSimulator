#include <stdio.h>
#include <stdlib.h>
#include "../include/cache.h"
#include "../include/config.h"

int main() {
    CacheConfig config;
    Cache *cache;

    // Read configuration
    printf("Reading configuration...\n");
    readConfigFile(&config);
    printf("Configuration loaded: numSets = %d\n", config.numSets);

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

        int hit = accessCache(cache, address, NULL);
        printf("Address 0x%X: %s\n", address, hit ? "Hit" : "Miss");
    }
    displayCache(cache);
    // Cleanup
    free(cache->cacheSets);
    free(cache);
    printf("Cache simulator exiting.\n");

    return 0;
}
