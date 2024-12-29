#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int numSets;
    int mainMemorySize;
    int linesPerSet;
} Config;

Config *readConfigFile(Config *config);

#endif 