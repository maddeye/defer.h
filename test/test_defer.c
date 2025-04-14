/**
 * @file test_defer.c
 * @brief Main test file for defer.h
 */

#include <stdio.h>
#include "test_common.h"

// Function declarations
void test_basic(void);
void test_memory_allocation(void);
void test_file_operations(void);
void test_nested(void);
void test_resource_management(void);
void test_array_allocation(void);
void test_zero_allocation(void);
void test_large_allocation(void);
void test_multiple_allocations(void);
void test_aligned_allocation(void);
void test_reallocation(void);
void test_custom_cleanup(void);
void test_allocation_errors(void);
void test_nested_scope_allocation(void);
void test_string_operations(void);
void test_array_operations(void);
void test_nested_resources(void);
void test_nested_resource_cleanup(void);
void test_temp_resource_cleanup(void);
void run_msvc_tests(void);

int main(void) {
    // Initialize sockets if needed
    if (init_sockets() != 0) {
        print_error("Failed to initialize sockets");
        return 1;
    }

    printf("Starting defer.h tests...\n");

    // Run basic tests
    printf("\n=== Running Basic Tests ===\n");
    test_basic();
    printf("\n");

    test_nested();
    printf("\n");

    test_early_return();
    printf("\n");

    test_multiple_defers();
    printf("\n");

    // Run memory tests
    printf("\n=== Running Memory Tests ===\n");
    test_zero_allocation();
    test_large_allocation();
    test_multiple_allocations();
    test_aligned_allocation();
    test_reallocation();
    test_custom_cleanup();
    test_allocation_errors();
    test_nested_scope_allocation();

    // Run file tests
    printf("\n=== Running Resource Tests ===\n");
    test_resource_management();
    test_array_allocation();
    test_file_operations();
    test_string_operations();
    test_array_operations();
    test_nested_resources();
    test_temp_resource_cleanup();

    // Run nested scope tests
    printf("\n=== Running Nested Scope Tests ===\n");
    test_nested();
    test_nested_resource_cleanup();


    // Real-world usage examples
    test_database_connection();
    printf("\n");

    test_mutex_locking();
    printf("\n");

    test_socket_handling();
    printf("\n");

    test_opengl_resources();
    printf("\n");

    printf("\nAll tests completed.\n");
    return 0;
} 