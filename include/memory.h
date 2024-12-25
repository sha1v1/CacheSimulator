#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    char **pageTable;  // Array of page pointers
    int totalSize;     // Total memory size in bytes
    int pageSize;      // Size of each page in bytes
    int numPages;      // Total number of pages
} Memory;

#endif