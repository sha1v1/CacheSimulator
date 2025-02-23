#include "../include/memory.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//initialize main memory
/**
 * @brief Initialize the Main memory Structure with user provided configuration.
 * 
 * @param memory A pointer to the Memory structure.
 * @param config A pointer to the Config Structure
 * 
 * Sets the total memory size, page size, calculates number of pages required. It also allocates memory for
 * the pageTable which is essentially an array of pointers to all these pages. These pointers are
 * initialized to NULL.
 */
void initializeMemory(Memory *memory, Config *config){
    memory->totalSize = config->mainMemorySize;
    memory->pageSize = 256; //fixed page size
    memory->numPages = (memory->totalSize + memory->pageSize - 1)/memory->pageSize; //round up

    //allocate memory for array of page pointers
    memory->pageTable = (char **)calloc(memory->numPages, sizeof(char *));
    if(!memory->pageTable){
        printf("Error: memoery allocation failed for page table\n");
        exit(1);
    }


}

/**
 * @brief Allocates (initializes) a memory page at the specified index.
 *
 * @param memory A pointer to the Memory structure.
 * @param pageIndex The index of the page to allocate.
 * @return int Returns:
 *              - 1 if the page was successfully allocated or already exists.
 *              - -1 if the memory is uninitialized.
 *              - -2 if the page index is out of bounds.
 *              - -3 if page allocation failed.
 *
 * Checks if the page has already been allocated. If not,
 * it dynamically allocates the page and populates it with random non-zero values.
 */
int allocatePage(Memory *memory, int pageIndex) {
    //check if memory has been initiliazed
    if(!memory || !memory->pageTable){
        printf("Error: Attempting to write to uninitialized memory\n");
        return -1;
    }

    //check if the page index lies within memory's bounds
    if(pageIndex >= memory->numPages || pageIndex < 0){
        printf("Error: Attempting to allocate page outside memory's bounds\n");
        return -2;
    }

    //check if the page has been initiliazed
    if (memory->pageTable[pageIndex] == NULL) {

        //allocate memory for the page
        memory->pageTable[pageIndex] = (char *)malloc(memory->pageSize * sizeof(char));
        if (!memory->pageTable[pageIndex]) {
            printf("Error: Page allocation failed.\n");
            return -3;
        }

        // Populate with random non-zero values
        for (int i = 0; i < memory->pageSize; i++) {
            memory->pageTable[pageIndex][i] = 32 + (rand() % (126 - 32 + 1));  // ASCII range [32, 126]
        }

        printf("Page %d allocated and initialized.\n", pageIndex);
    }
    return 1;
}


/**
 * @brief Given a memory address, it returns the value at the said address.
 * @param memory A pointer to the Memory struct
 * @param address The address to read from
 * 
 * cacluate the page and offset from the given address. If the page hasn't been allocated (initialized),
 * it is dynamically polulated with random values and the value at the given address is returned. 
*/
int readFromMemory(Memory *memory, int address){
    if(!memory || !memory->pageTable){
        printf("Error: Attempting to write to uninitialized memory\n");
        return -1;
    }
    
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

/**
 * @brief Writes a value to a given address in memory.
 * 
 * @param memory A pointer to the memory structure.
 * @param address The address to write to
 * @param value The value to write to that address
 * 
 * Calculate the page index and offset from the address. If page hasn't been allocated (initialized) yet,
 * it is loaded with random values and the memory address is then written to with the provided value.
 */
int writeToMemory(Memory *memory, int address, char value){
    if(!memory || !memory->pageTable){
        printf("Error: Attempting to write to uninitialized memory\n");
        return 0;
    }

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
    printf("Memory at address %d changed to %d\n", address, value);
    return 1;
}


//since I am calling malloc inside, the caller will have to free the memory.
/**
 * @brief fetches a block of data (cache line) from memory.
 * 
 * @param memory A pointer to the Memory structure
 * @param addr The address that triggered the fetching of the data block
 * @returns blockData: An array containing the block data.
 * 
 * Figure out the starting address of the block from the given address and fetch 32 bytes of data
 * from the computed address. In case the page hasn't been allocated yet, initialize it first.
 */
char *fetchBlockFromMemory(Memory *memory, unsigned int addr) {
    if(addr >= memory->totalSize|| addr < 0){
        printf("Invalid Memory address %d\n", addr);
        exit(0);
    }

    int blockStartAddr = addr & ~31;  // Align to block start
    char *blockData = (char *)malloc(32 * sizeof(char));
    for (int i = 0; i < 31; i++) {
        blockData[i] = readFromMemory(memory, blockStartAddr + i);
    }
    blockData[31] = '\0';
    return blockData;
}

/**
 * @brief Frees all memory occupied by the Memory structure.
 *
 * @param memory A pointer to the Memory structure to free.
 *
 * This function frees all allocated pages and the page table itself.
 * After calling this, the Memory structure will be in an
 * uninitialized state.
 */
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

