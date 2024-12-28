#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int numSets;
    int mainMemorySize;
} Config;

Config *readConfigFile(Config *config);

#endif 