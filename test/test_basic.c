/**
 * @file test_basic.c
 * @brief Basic tests for defer.h functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_common.h"
#include "../defer.h"

// Test functions for basic defer functionality
static void cleanup1(void* arg) {
    (void)arg;  // Unused
    printf("Cleanup 1 executed\n");
}

static void cleanup2(void* arg) {
    (void)arg;  // Unused
    printf("Cleanup 2 executed\n");
}

// Test basic defer functionality
void test_basic_defer(void) {
    printf("\n=== Testing Basic Defer ===\n");
    printf("Entering test_basic_defer\n");
    
    int dummy = 0;
    defer(cleanup1, &dummy);
    printf("First defer registered\n");
    
    {  // Testing nested scope
        int dummy2 = 0;
        defer(cleanup2, &dummy2);
        printf("Second defer registered in nested scope\n");
    }  // cleanup2 should execute here
    
    printf("Exiting test_basic_defer\n");
}  // cleanup1 should execute here

// Cleanup functions for multiple defers test
static void cleanup1_multiple(void* arg) {
    int* value = (int*)arg;
    printf("Cleanup 1 executed\n");
    *value = 1;
}

static void cleanup2_multiple(void* arg) {
    int* value = (int*)arg;
    printf("Cleanup 2 executed\n");
    *value = 2;
}

void test_basic_memory(void) {
    void* ptr = malloc(100);
    if (!ptr) {
        print_error("Memory allocation failed");
        return;
    }
    defer_free(ptr);

    memset(ptr, 0, 100);
    print_success("Basic memory test completed");
}

void test_basic_file(void) {
    FILE* file = fopen("build/test.txt", "w");
    if (!file) {
        printf("Failed to create test file\n");
        return;
    }
    defer_fclose(file);

    fprintf(file, "Test content\n");
    print_success("Basic file test completed");
}

void test_basic_string(void) {
    char* str = strdup("Basic string test");
    if (!str) {
        print_error("String allocation failed");
        return;
    }
    defer_free(str);

    print_success("Basic string test completed");
}

void test_multiple_defers(void) {
    printf("\n=== Testing Multiple Defers ===\n");
    printf("Entering multiple defers test\n");
    
    int dummy1 = 0, dummy2 = 0;
    defer(cleanup1_multiple, &dummy1);
    defer(cleanup2_multiple, &dummy2);
    
    printf("Both defers registered\n");
    print_success("Multiple defers test completed");
} 