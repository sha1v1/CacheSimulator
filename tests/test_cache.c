#include "../src/unity/unity.h"
#include "../include/cache.h"
#include <string.h>

Cache *cache;

void setUp(void){
    cache = initalizeCache(4);
}

void tearDown(void){
    freeCache(cache);
}

void test_initialize_cache(void){
    TEST_ASSERT_NOT_NULL(cache);
    TEST_ASSERT_EQUAL(4, cache->numSets);
    TEST_ASSERT_NOT_NULL(cache->cacheSets);
    for (int i = 0; i < 4; i++) {
        Line line = cache->cacheSets[i].cacheLine;
        TEST_ASSERT_FALSE(line.validBit);
        TEST_ASSERT_EQUAL(0, line.tag);
        TEST_ASSERT_EQUAL_STRING("", line.block);
    }

    // freeCache(cache);
}

void test_cache_hit(void){
    int setIndex = 2;
    int tagBits = 5;
    strcpy(cache->cacheSets[setIndex].cacheLine.block, "TestData");
    cache->cacheSets[setIndex].cacheLine.validBit = true;
    cache->cacheSets[setIndex].cacheLine.tag = tagBits;

    //an address that maps to the same set and has the same tagbits
    unsigned int addr = (tagBits << 7) | (setIndex << 5);  // 7 = log2(32) + log2(4)
    int result = accessCache(cache, addr);

    TEST_ASSERT_EQUAL(1, result);  // Cache hit

}

void test_cache_miss(void) {
    // Generate an address that does not match any cache line
    unsigned int addr = 100;  // Arbitrary address
    int result = accessCache(cache, addr);

    TEST_ASSERT_EQUAL(0, result);  // Cache miss
}

void test_update_cache(void) {
    int setIndex = 1;
    int tagBits = 6;
    const char *blockData = "NewData";

    updateCache(cache, setIndex, tagBits, blockData);

    Line line = cache->cacheSets[setIndex].cacheLine;
    TEST_ASSERT_TRUE(line.validBit);
    TEST_ASSERT_EQUAL(tagBits, line.tag);
    TEST_ASSERT_EQUAL_STRING(blockData, line.block);
}


void test_invalid_cache_access(void) {
    Cache *invalidCache = NULL;
    unsigned int addr = 50;

    int result = accessCache(invalidCache, addr);

    TEST_ASSERT_EQUAL(-1, result);  // Expect cache access to fail gracefully
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initialize_cache);
    RUN_TEST(test_cache_hit);
    RUN_TEST(test_cache_miss);
    RUN_TEST(test_update_cache);
    RUN_TEST(test_invalid_cache_access);

    return UNITY_END();
}