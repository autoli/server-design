#include "../autoli/autoli.h"
#include <assert.h>

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void test_assert() {
    AUTOLI_LOG_INFO(g_logger) << autoli::BacktraceToString(10);
    //AUTOLI_ASSERT2(0 == 1, "abcdef xx");
}

int main(int argc, char** argv) {
    test_assert();
    
    //int arr[] = {1,3,5,7,9,11};

    // AUTOLI_LOG_INFO(g_logger) << autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 0);
    // AUTOLI_LOG_INFO(g_logger) << autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 1);
    // AUTOLI_LOG_INFO(g_logger) << autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 4);
    // AUTOLI_LOG_INFO(g_logger) << autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 13);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 0) == -1);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 1) == 0);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 2) == -2);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 3) == 1);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 4) == -3);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 5) == 2);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 6) == -4);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 7) == 3);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 8) == -5);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 9) == 4);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 10) == -6);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 11) == 5);
    // AUTOLI_ASSERT(autoli::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 12) == -7);
    return 0;
}
