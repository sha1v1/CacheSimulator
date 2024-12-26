#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    char **pageTable;  // Array of page pointers
    int totalSize;     // Total memory size in bytes
    int pageSize;      // Size of each page in bytes
    int numPages;      // Total number of pages
} Memory;

void initializeMemory(Memory *memory, CacheConfig *config);
void allocatePage(Memory *memory, int pageIndex);
char readFromMemory(Memory *memory, int address);
int writeToMemory(Memory *memory, int address, char value);
void freeMemory(Memory *memory);

#endif