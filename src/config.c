#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



void readConfigFile(Config *config){

    char cwd[1024];
if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working directory: %s\n", cwd);
}

    //open config file
    FILE* configFile = fopen("config.txt", "r");
    if(!configFile){
        printf("Could not open config.txt\n");
        exit(1);
    }
    
    char buffer[50];
    //fgets reads a line and stores in buffer after terminating it with \0
    //i.e. buffer stores a valid string 
    while(fgets(buffer, sizeof(buffer), configFile)){
        
        //to store the key-value pair in the current line
        char key[25], value[25];
        sscanf(buffer, "%24[^=]=%24s", key, value);
        if(strcmp(key, "num_sets") == 0){
            config->numSets = atoi(value);
            printf("Read the number of sets\n");
        }
        else if(strcmp(key, "main_memory_size") == 0){
            config->mainMemorySize = atoi(value);
            printf("Red main memory size\n");
        }
        else if(strcmp(key, "lines_per_set") == 0){
            config->linesPerSet = atoi(value);
            printf("read lines per set\n");
        }
        else if(strcmp(key, "replacement_policy") == 0){
            strcpy(config->replacementPolicy, value);
            printf("Read the replacement policy\n");
        }
        else if(strcmp(key,"write_policy") == 0){
            strcpy(config->writePolicy, value);
            printf("read write policy\n");
        }
        else{
            printf("Unknown key: %s\n", key);
            exit(1);
        }

    }
    printf("Read config file.\n");
    return;
}