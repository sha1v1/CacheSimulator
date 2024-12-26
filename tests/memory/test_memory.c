#include "../../include/memory.h"
#include "../../include/config.h"
#include "../../src/unity/unity.h"
#include "../../include/memory.h"


Memory memory;
CacheConfig config;

void setUp(void){
    config.mainMemorySize = 1024;
    initializeMemory(&memory, &config);
}

void tearDown(void){
    freeMemory(&memory);
}

void test_initialize_memory(){
    TEST_ASSERT_NOT_NULL(memory.pageTable);
    TEST_ASSERT_EQUAL(4, memory.numPages);
    TEST_ASSERT_EQUAL(256, memory.pageSize);
    TEST_ASSERT_EQUAL(1024, memory.totalSize);


}

void test_read_from_memory(void){
    char value = readFromMemory(&memory, 500);
    TEST_ASSERT(value >= 1 && value <= 255);
}

void test_write_to_memory(void){
    writeToMemory(&memory, 500, 42);
    char value = readFromMemory(&memory, 500);
    TEST_ASSERT_EQUAL(42, value);

}

void test_out_of_bounds_access(void) {
    char readValue = readFromMemory(&memory, 2000);  // Out of bounds
    TEST_ASSERT_EQUAL(-1, readValue);               // Should return -1

    int writeSuccess = writeToMemory(&memory, 2000, 42);
    TEST_ASSERT_EQUAL(0, writeSuccess);             // Should fail
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initialize_memory);
    RUN_TEST(test_read_from_memory);
    RUN_TEST(test_write_to_memory);
    RUN_TEST(test_out_of_bounds_access);

    return UNITY_END();
}
