#ifndef MEMORY_H
#define MEMORY_H
#include "config.h"

typedef struct {
    char **pageTable;  // Array of page pointers
    int totalSize;     // Total memory size in bytes
    int pageSize;      // Size of each page in bytes
    int numPages;      // Total number of pages
} Memory;

void initializeMemory(Memory *memory, Config *config);
int allocatePage(Memory *memory, int pageIndex);
int readFromMemory(Memory *memory, int address);
int writeToMemory(Memory *memory, int address, char value);
int fetchBlockFromMemory(Memory *memory, unsigned int addr, char **blockData);
void freeMemory(Memory *memory);

#endif