#include "../src/unity/unity.h"
#include "../include/cache.h"
#include <string.h>
#include <stdlib.h>

Cache *cache;

void setUp(void) {
    // Initialize a cache with 4 sets and 2 lines per set
    Config config = {4, 1024, 2}; 
    cache = initalizeCache(&config);
}

void tearDown(void) {
    freeCache(cache);
}

void test_initialize_cache(void) {
    TEST_ASSERT_NOT_NULL(cache);
    TEST_ASSERT_EQUAL(4, cache->numSets);
    TEST_ASSERT_EQUAL(2, cache->linesPerSet);  // Two lines per set
    TEST_ASSERT_NOT_NULL(cache->cacheSets);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            Line line = cache->cacheSets[i].cacheLines[j];
            TEST_ASSERT_FALSE(line.validBit);
            TEST_ASSERT_EQUAL(0, line.tag);
            TEST_ASSERT_EQUAL_STRING("", line.block);
        }
    }
}

void test_cacheAccess(void) {
    unsigned int address = 0x1234;
    char outData;

    // Check miss
    int result = checkCache(cache, address, &outData);
    TEST_ASSERT_EQUAL(0, result);

    // Simulate fetching data and updating cache
    updateCache(&cache->cacheSets[getSetIndex(address, 4)].cacheLines[0],
                getTagBits(address, 4),
                "BlockData");

    // Check hit
    result = checkCache(cache, address, &outData);
    TEST_ASSERT_EQUAL(1, result);

}


void test_update_cache(void) {
    int setIndex = 1;
    int lineIndex = 0;
    int tagBits = 6;
    const char *blockData = "NewData";

    Line *line = &cache->cacheSets[setIndex].cacheLines[lineIndex];
    updateCache(line, tagBits, blockData);

    TEST_ASSERT_TRUE(line->validBit);
    TEST_ASSERT_EQUAL(tagBits, line->tag);
    TEST_ASSERT_EQUAL_STRING(blockData, line->block);
}

void test_RandomReplacement(void){
    unsigned int address1 = 0x1234; // Maps to set 0, line 0
    unsigned int address2 = 0x5678; // Maps to set 0, line 1
    unsigned int address3 = 0x9ABC; // Maps to set 0, should replace a random line

    // Access first two addresses (fills both lines)
    checkCache(cache, address1, NULL);
    updateCache(handleLineReplacement(cache, address1, "RANDOM"), getTagBits(address1, 4), "Block1");

    checkCache(cache, address2, NULL);
    updateCache(handleLineReplacement(cache, address2, "RANDOM"), getTagBits(address2, 4), "Block2");

    // Access a new address to trigger replacement
    checkCache(cache, address3, NULL);
    Line *replacedLine = handleLineReplacement(cache, address3, "RANDOM");

    // Verify Random replacement
    // Ensure that the tag of the replaced line matches one of the first two tags
    unsigned int tag1 = getTagBits(address1, 4);
    unsigned int tag2 = getTagBits(address2, 4);
    TEST_ASSERT_TRUE(replacedLine->tag == tag1 || replacedLine->tag == tag2);


}


void test_LRUReplacement(void) {
    unsigned int address1 = 0x1234; // Maps to set 0, line 0
    unsigned int address2 = 0x5678; // Maps to set 0, line 1
    unsigned int address3 = 0x9ABC; // Maps to set 0, should replace line 0 (LRU)

    // Access first two addresses (fills both lines)
    checkCache(cache, address1, NULL);
    updateCache(handleLineReplacement(cache, address1, "LRU"), getTagBits(address1, 4), "Block1");

    checkCache(cache, address2, NULL);
    updateCache(handleLineReplacement(cache, address2, "LRU"), getTagBits(address2, 4), "Block2");

    // Access a new address to trigger replacement
    checkCache(cache, address3, NULL);
    Line *replacedLine = handleLineReplacement(cache, address3, "LRU");

    // Verify LRU replacement
    TEST_ASSERT_EQUAL(getTagBits(address1, 4), replacedLine->tag); // Line 0 should be replaced

    
}



void test_invalid_cache_access(void) {
    Cache *invalidCache = NULL;
    unsigned int addr = 50;

    int result = checkCache(invalidCache, addr, NULL);
    TEST_ASSERT_EQUAL(-1, result);  // Expect cache access to fail gracefully
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initialize_cache);
    RUN_TEST(test_cacheAccess);
    RUN_TEST(test_update_cache);
    RUN_TEST(test_RandomReplacement);
    RUN_TEST(test_LRUReplacement);
    RUN_TEST(test_invalid_cache_access);

    return UNITY_END();
}
