#include "../include/memory.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//initialize main memory
void initializeMemory(Memory *memory, CacheConfig *config){
    memory->totalSize = config->mainMemorySize;
    memory->pageSize = 256; //fixed page size
    memory->numPages = (memory->totalSize + memory->pageSize - 1)/memory->pageSize; //round up

    //allocate memory for array of page pointers
    memory->pageTable = (char **)calloc(memory->numPages, sizeof(char *));
    if(!memory->pageTable){
        printf("Error: memoery allocation failed for page table");
        exit(1);
    }


}

void allocatePage(Memory *memory, int pageIndex) {
    //check if the page has been initiliazed
    if (memory->pageTable[pageIndex] == NULL) {

        //allocate memory for the page
        memory->pageTable[pageIndex] = (char *)malloc(memory->pageSize * sizeof(char));
        if (!memory->pageTable[pageIndex]) {
            printf("Error: Page allocation failed.\n");
            exit(1);
        }

        // Populate with random non-zero values
        for (int i = 0; i < memory->pageSize; i++) {
            memory->pageTable[pageIndex][i] = (rand() % 255) + 1;  // Values between 1 and 255
        }

        printf("Page %d allocated and initialized.\n", pageIndex);
    }
}

char readFromMemory(Memory *memory, int address){
    int pageIdx = address/memory->pageSize; //page index where the address is
    int offset = address % memory->pageSize; //offset within that page
    
    if(address >= memory->totalSize|| address < 0){
        printf("Invalid Memory address %d\n", address);
        return -1;
    }

    //populate page if this is the first time an address from this page has been accessed
    allocatePage(memory, pageIdx);

    //otherwise return the value at the given address
    return memory->pageTable[pageIdx][offset];
}

int writeToMemory(Memory *memory, int address, char value){
    int pageIdx =  address/memory->pageSize;
    int offset = address % memory->pageSize;
    
    if (address >= memory->totalSize || address < 0) {
        printf("Error: Invalid memory address %d.\n", address);
        return 0;
    }
    //incase the page we're writing to hadn't been initialized
    if (memory->pageTable[pageIdx] == NULL){
        memory->pageTable[pageIdx] = (char *)malloc(memory->pageSize * sizeof(char));
        if (!memory->pageTable[pageIdx]) {
            printf("Error: Page allocation failed.\n");
            exit(1);
        }
    }
    memory->pageTable[pageIdx][offset] = value;
    printf("Memory at address %d changed to %d", address, value);
    return 1;
}

void freeMemory(Memory *memory){

    if (!memory || !memory->pageTable) {
        printf("Memory already freed or not initialized.\n");
        return;
    }

    for (int i = 0; i < memory->numPages; i++) {
        if (memory->pageTable[i] != NULL) {
            free(memory->pageTable[i]);  //free eacch allocated page
        }
    }
    free(memory->pageTable);  //free the page table
    memory->pageTable = NULL;

}

