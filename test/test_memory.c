#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdalign.h>
#include "test_common.h"
#include "../defer.h"

typedef struct {
    void* data;
    size_t size;
} custom_data_t;

// Custom cleanup function for test_custom_cleanup
static void cleanup_custom(void* arg) {
    custom_data_t* data = (custom_data_t*)arg;
    free(data->data);
    free(data);
}

// Test memory allocation with zero size
void test_zero_allocation(void) {
    printf("\nRunning zero allocation test...\n");
    
    // Allocate zero bytes
    void* ptr = malloc(0);
    if (ptr) {
        defer_free(ptr);
        print_success("Zero allocation test completed");
    } else {
        print_error("Zero allocation failed");
    }
}

// Test memory allocation with very large size
void test_large_allocation(void) {
    printf("\nRunning large allocation test...\n");
    
    // Try to allocate a very large block
    size_t large_size = SIZE_MAX / 2;
    void* ptr = malloc(large_size);
    if (ptr) {
        defer_free(ptr);
        print_success("Large allocation test completed");
    } else {
        // Expected to fail on most systems
        print_success("Large allocation failed (expected)");
    }
}

// Test multiple allocations in a single scope
void test_multiple_allocations(void) {
    printf("\nRunning multiple allocations test...\n");
    
    // Allocate multiple blocks
    void* ptr1 = malloc(100);
    if (!ptr1) {
        print_error("First allocation failed");
        return;
    }
    defer_free(ptr1);

    void* ptr2 = malloc(200);
    if (!ptr2) {
        print_error("Second allocation failed");
        return;
    }
    defer_free(ptr2);

    void* ptr3 = malloc(300);
    if (!ptr3) {
        print_error("Third allocation failed");
        return;
    }
    defer_free(ptr3);

    print_success("Multiple allocations test completed");
}

// Test memory allocation with alignment
void test_aligned_allocation(void) {
    printf("\nRunning aligned allocation test...\n");
    
    // Allocate aligned memory
    void* ptr = aligned_alloc(16, 128);
    if (!ptr) {
        print_error("Aligned allocation failed");
        return;
    }
    defer_free(ptr);

    // Verify alignment
    if ((uintptr_t)ptr % 16 != 0) {
        print_error("Memory not properly aligned");
        return;
    }

    print_success("Aligned allocation test completed");
}

// Test memory reallocation
void test_reallocation(void) {
    printf("\nRunning reallocation test...\n");
    
    // Initial allocation
    void* ptr = malloc(100);
    if (!ptr) {
        print_error("Initial allocation failed");
        return;
    }
    defer_free(ptr);

    // Reallocate to larger size
    void* new_ptr = realloc(ptr, 200);
    if (!new_ptr) {
        print_error("Reallocation failed");
        return;
    }
    ptr = new_ptr;

    print_success("Reallocation test completed");
}

// Test memory allocation with custom cleanup
void test_custom_cleanup(void) {
    printf("\nRunning custom cleanup test...\n");
    
    typedef struct {
        int* data;
        size_t size;
    } custom_data_t;

    custom_data_t* custom = (custom_data_t*)malloc(sizeof(custom_data_t));
    if (!custom) {
        print_error("Custom data allocation failed");
        return;
    }

    custom->size = 100;
    custom->data = (int*)malloc(custom->size);
    if (!custom->data) {
        free(custom);
        print_error("Custom data buffer allocation failed");
        return;
    }

    defer(cleanup_custom, custom);

    print_success("Custom cleanup test completed");
}

// Test memory allocation with error handling
void test_allocation_errors(void) {
    // Use a very large but not maximum size
    const size_t large_size = SIZE_MAX / 2;
    void* ptr = malloc(large_size);
    if (!ptr) {
        print_error("Error occurred during allocation (expected)");
    } else {
        defer_free(ptr);
        print_error("Unexpected success in allocation");
    }

    ptr = malloc(100);
    if (!ptr) {
        print_error("Normal allocation failed");
        return;
    }

    defer_free(ptr);
    print_success("Allocation errors test completed");
}

// Test memory allocation with nested scopes
void test_nested_scope_allocation(void) {
    printf("\nRunning nested scope allocation test...\n");
    
    void* outer = malloc(100);
    if (!outer) {
        print_error("Outer allocation failed");
        return;
    }
    defer_free(outer);

    {
        void* inner = malloc(50);
        if (!inner) {
            print_error("Inner allocation failed");
            return;
        }

        defer_free(inner);
        print_success("Inner scope allocation completed");
    }

    print_success("Nested scope allocation test completed");
} 