#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int numSets;
} CacheConfig;

CacheConfig *readConfigFile(CacheConfig *config);

#endif 