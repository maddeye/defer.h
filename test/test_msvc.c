/**
 * @file test_msvc.c
 * @brief MSVC-specific tests for defer.h
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_common.h"

#define DEFER_IMPLEMENTATION
#include "../defer.h"

static void cleanup1(void* arg) {
    (void)arg;  // Unused
    printf("Cleanup 1 executed\n");
}

static void cleanup2(void* arg) {
    (void)arg;  // Unused
     printf("Cleanup 2 executed\n");
}

// Test basic MSVC defer functionality
void test_msvc_basic(void) {
    int dummy = 0;
    defer(cleanup1, &dummy);
    printf("First defer registered\n");

    {  // Testing nested scope
        int dummy2 = 0;
        defer(cleanup2, &dummy2);
        printf("Second defer registered in nested scope\n");
    }  // cleanup2 should execute here

    print_success("Basic MSVC defer test completed");
}

// Test nested scopes with MSVC
void test_msvc_nested(void) {
    printf("\n=== Testing MSVC Nested Scopes ===\n");
    printf("Starting nested test...\n");
    
    // First test: outer scope only
    void* outer_ptr = NULL;
    {
        printf("Testing outer scope...\n");
        int* outer = (int*)malloc(sizeof(int));
        if (!outer) {
            print_error("Outer allocation failed");
            return;
        }
        outer_ptr = (void*)outer;  // Save pointer for verification
        printf("Allocated outer at %p\n", outer_ptr);
        defer_free(outer);
        
        *outer = 200;
        printf("Outer value: %d\n", *outer);
    }  // defer_free should execute here
    
    // Verify that outer was freed
    {
        int* test = (int*)malloc(sizeof(int));
        if (!test) {
            print_error("Test allocation failed");
            return;
        }
        printf("Allocated test at %p\n", (void*)test);
        // If outer was properly freed, this allocation should get a different address
        if ((void*)test == outer_ptr) {
            print_error("Memory was not properly freed");
            return;
        }
        free(test);
    }
    
    // Second test: inner scope only
    void* inner_ptr = NULL;
    {
        printf("Testing inner scope...\n");
        int* inner = (int*)malloc(sizeof(int));
        if (!inner) {
            print_error("Inner allocation failed");
            return;
        }
        inner_ptr = (void*)inner;  // Save pointer for verification
        printf("Allocated inner at %p\n", inner_ptr);
        defer_free(inner);
        
        *inner = 100;
        printf("Inner value: %d\n", *inner);
    }  // defer_free should execute here
    
    // Third test: nested scopes
    void* nested_outer_ptr = NULL;
    void* nested_inner_ptr = NULL;
    {
        printf("Testing nested scopes...\n");
        int* outer = (int*)malloc(sizeof(int));
        if (!outer) {
            print_error("Outer allocation failed");
            return;
        }
        nested_outer_ptr = (void*)outer;  // Save pointer for verification
        printf("Allocated outer at %p\n", nested_outer_ptr);
        defer_free(outer);
        
        {
            int* inner = (int*)malloc(sizeof(int));
            if (!inner) {
                print_error("Inner allocation failed");
                return;
            }
            nested_inner_ptr = (void*)inner;  // Save pointer for verification
            printf("Allocated inner at %p\n", nested_inner_ptr);
            defer_free(inner);
            
            *inner = 100;
            printf("Inner value: %d\n", *inner);
        }  // defer_free should execute here for inner
        
        // Verify that inner was freed
        {
            int* test = (int*)malloc(sizeof(int));
            if (!test) {
                print_error("Test allocation failed");
                return;
            }
            printf("Allocated test at %p\n", (void*)test);
            // If inner was properly freed, this allocation should get a different address
            if ((void*)test == nested_inner_ptr) {
                print_error("Memory was not properly freed in nested scope");
                return;
            }
            free(test);
        }
        
        *outer = 200;
        printf("Outer value: %d\n", *outer);
    }  // defer_free should execute here for outer
    
    // Final verification of nested outer
    {
        int* test = (int*)malloc(sizeof(int));
        if (!test) {
            print_error("Test allocation failed");
            return;
        }
        printf("Allocated test at %p\n", (void*)test);
        // If outer was properly freed, this allocation should get a different address
        if ((void*)test == nested_outer_ptr) {
            print_error("Memory was not properly freed in nested scope");
            return;
        }
        free(test);
    }
    
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

// Entry point
int main(void) {
    run_msvc_tests();
    return 0;
} 