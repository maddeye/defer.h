/**
 * @file test_defer.c
 * @brief Main test file for defer.h
 */

#include <stdio.h>
#include "test_common.h"
#include "test_basic.h"
#include "test_memory.h"
#include "test_files.h"
#include "test_cases.h"
#include "test_resources.h"
#include "test_msvc.h"

int main(void) {
    // Initialize sockets if needed
    if (init_sockets() != 0) {
        print_error("Failed to initialize sockets");
        return 1;
    }

    printf("Starting defer.h tests...\n");

    // Run basic tests
    test_basic();
    printf("\n");

    test_nested();
    printf("\n");

    test_early_return();
    printf("\n");

    test_multiple_defers();
    printf("\n");

    // Run memory tests
    test_memory();
    printf("\n");

    test_basic_memory();
    printf("\n");

    test_resource_cleanup();
    printf("\n");

    test_error_handling();
    printf("\n");

    test_zero_allocation();
    printf("\n");

    test_large_allocation();
    printf("\n");

    test_multiple_allocations();
    printf("\n");

    test_aligned_allocation();
    printf("\n");

    test_reallocation();
    printf("\n");

    test_custom_cleanup();
    printf("\n");

    test_allocation_errors();
    printf("\n");

    test_nested_scope_allocation();
    printf("\n");

    // Run file tests
    test_files();
    printf("\n");

    // Run case tests
    test_cases();
    printf("\n");

    // Run resource tests
    test_resources();
    printf("\n");

    // Run MSVC-specific tests
    #ifdef _MSC_VER
    run_msvc_tests();
    #endif

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