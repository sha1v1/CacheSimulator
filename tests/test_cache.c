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

void test_cache_hit_random_replacement(void) {
    int setIndex = 2;
    int lineIndex = 1;
    int tagBits = 5;

    // Preload a cache line to simulate a valid entry
    Line *line = &cache->cacheSets[setIndex].cacheLines[lineIndex];
    strcpy(line->block, "TestData");
    line->validBit = true;
    line->tag = tagBits;

    // Address that maps to the same set and has the same tag bits
    unsigned int addr = (tagBits << 7) | (setIndex << 5);  // 7 = log2(32) + log2(4)
    int result = handleCacheAccess(cache, addr, NULL, "RANDOM");

    TEST_ASSERT_EQUAL(1, result);  // Cache hit
}

void test_cache_miss_random_replacement(void) {
    unsigned int addr = 100;  //arbitrary address
    int result = handleCacheAccess(cache, addr, "NewBlockData", "RANDOM");

    TEST_ASSERT_EQUAL(0, result);  //cache miss

    //verify that the block was loaded into the cache
    int setIndex = getSetIndex(addr, cache->numSets);
    Line *line = &cache->cacheSets[setIndex].cacheLines[0];
    TEST_ASSERT_TRUE(line->validBit);
    TEST_ASSERT_EQUAL_STRING("NewBlockData", line->block);
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

void test_random_replacement(void) {
    unsigned int addr1 = 0x64;  //maps to set 1
    unsigned int addr2 = 0x128; //also maps to set 1 (different tag)
    unsigned int addr3 = 0x1F4; //another one for set 1; trigger line replacement

    handleCacheAccess(cache, addr1, "Block1", "RANDOM");
    handleCacheAccess(cache, addr2, "Block2", "RANDOM");
    handleCacheAccess(cache, addr3, "Block3", "RANDOM");  // Should trigger random replacement

    // Check that exactly one line in the set contains Block3
    int setIndex = getSetIndex(addr1, cache->numSets);
    int validCount = 0;
    for (int i = 0; i < cache->linesPerSet; i++) {
        Line *line = &cache->cacheSets[setIndex].cacheLines[i];
        if (line->validBit && strcmp(line->block, "Block3") == 0) {
            validCount++;
        }
    }
    TEST_ASSERT_EQUAL(1, validCount);
}

void test_cache_hit_updates_lastAccessTime(void){
    int setIndex = 1;
    int lineIndex = 0;
    int tagBits = 10;

    Line *line = &cache->cacheSets[setIndex].cacheLines[lineIndex];
    strcpy(line->block, "TestData");
    line->validBit = true;
    line->tag = tagBits;
    line->lastAccessTime = 5;

    unsigned int addr = (tagBits << 7) | (setIndex << 5);
    int result = handleCacheAccess(cache, addr, NULL, "LRU");

    TEST_ASSERT_EQUAL(1, result);
    TEST_ASSERT_EQUAL(1, line->lastAccessTime);
}


void test_lru_replacement(void) {
    unsigned int addr1 = 0x64;
    unsigned int addr2 = 0x128;
    unsigned int addr3 = 0x1F4;
    handleCacheAccess(cache, addr1, "Block1", "LRU");
    handleCacheAccess(cache, addr2, "Block2", "LRU");
    handleCacheAccess(cache, addr3, "Block3", "LRU");

    int setIndex = getSetIndex(addr1, cache->numSets);
    Line *lines = cache->cacheSets[setIndex].cacheLines;

    TEST_ASSERT_EQUAL_STRING("Block3", lines[0].block);
    TEST_ASSERT(lines[0].lastAccessTime > lines[1].lastAccessTime);
}


void test_invalid_cache_access(void) {
    Cache *invalidCache = NULL;
    unsigned int addr = 50;

    int result = handleCacheAccess(invalidCache, addr, "TestBlock", "RANDOM");

    TEST_ASSERT_EQUAL(-1, result);  // Expect cache access to fail gracefully
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initialize_cache);
    RUN_TEST(test_cache_hit_random_replacement);
    RUN_TEST(test_cache_miss_random_replacement);
    RUN_TEST(test_update_cache);
    RUN_TEST(test_random_replacement);
    RUN_TEST(test_invalid_cache_access);

    return UNITY_END();
}
