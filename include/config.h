#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int numSets;
    int mainMemorySize;
    int linesPerSet;
    char replacementPolicy[10];
    char writePolicy[15];
} Config;

Config *readConfigFile(Config *config);

#endif 