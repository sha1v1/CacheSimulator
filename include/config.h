#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int numSets;
    int mainMemory;
} CacheConfig;

CacheConfig *readConfigFile(CacheConfig *config);

#endif 