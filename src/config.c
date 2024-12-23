#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


CacheConfig *readConfigFile(CacheConfig *config){
    //open config file
    FILE* configFile = fopen("config.txt", "r");
    if(!configFile){
        printf("Could not open config.txt");
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
        }
        else{
            printf("Unknown key: %s\n", key);
            exit(1);
        }

    }
    return config;

}