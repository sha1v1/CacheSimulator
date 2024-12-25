#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int numSets;
    int mainMemorySize;
} CacheConfig;

CacheConfig *readConfigFile(CacheConfig *config);

#endif 