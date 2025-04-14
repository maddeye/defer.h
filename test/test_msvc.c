/**
 * @file test_msvc.c
 * @brief MSVC-specific tests for defer.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_common.h"
#include "../defer.h"

// Test basic MSVC defer functionality
void test_msvc_basic(void) {
    printf("\n=== Testing MSVC Basic Defer ===\n");
    
    int* ptr = (int*)malloc(sizeof(int));
    if (!ptr) {
        print_error("Memory allocation failed");
        return;
    }
    defer_free(ptr);
    
    *ptr = 42;
    printf("Value before cleanup: %d\n", *ptr);
    print_success("Basic MSVC defer test completed");
}

// Test nested scopes with MSVC
void test_msvc_nested(void) {
    printf("\n=== Testing MSVC Nested Scopes ===\n");
    
    int* outer = (int*)malloc(sizeof(int));
    if (!outer) {
        print_error("Outer allocation failed");
        return;
    }
    defer_free(outer);
    
    {
        int* inner = (int*)malloc(sizeof(int));
        if (!inner) {
            print_error("Inner allocation failed");
            return;
        }
        defer_free(inner);
        
        *inner = 100;
        printf("Inner value before cleanup: %d\n", *inner);
    }
    
    *outer = 200;
    printf("Outer value before cleanup: %d\n", *outer);
    print_success("Nested scopes test completed");
}

// Test file operations with MSVC
void test_msvc_files(void) {
    printf("\n=== Testing MSVC File Operations ===\n");
    
    FILE* file = fopen("msvc_test.txt", "w");
    if (!file) {
        print_error("Failed to open file");
        return;
    }
    defer_fclose(file);
    
    fprintf(file, "MSVC test content\n");
    print_success("File operations test completed");
}

// Test multiple resources in same scope
void test_msvc_multiple(void) {
    printf("\n=== Testing MSVC Multiple Resources ===\n");
    
    // Allocate multiple resources
    int* array = (int*)malloc(10 * sizeof(int));
    if (!array) {
        print_error("Array allocation failed");
        return;
    }
    defer_free(array);
    
    FILE* file = fopen("msvc_multiple.txt", "w");
    if (!file) {
        print_error("File open failed");
        return;
    }
    defer_fclose(file);
    
    // Use the resources
    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }
    fprintf(file, "Multiple resources test\n");
    
    print_success("Multiple resources test completed");
}

// Test error handling with MSVC
void test_msvc_errors(void) {
    printf("\n=== Testing MSVC Error Handling ===\n");
    
    FILE* file = fopen("nonexistent/path/file.txt", "r");
    if (!file) {
        print_success("Expected file open failure handled correctly");
        return;
    }
    defer_fclose(file);
    
    print_error("Unexpected file open success");
}

// Main test function
void run_msvc_tests(void) {
    printf("\nRunning MSVC-specific tests...\n");
    
    test_msvc_basic();
    test_msvc_nested();
    test_msvc_files();
    test_msvc_multiple();
    test_msvc_errors();
    
    printf("\nMSVC tests completed\n");
} 