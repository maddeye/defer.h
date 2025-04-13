/**
 * @file test_defer.c
 * @brief Main test file for defer.h
 */

#include "test_common.h"

int main(void) {
    // Initialize sockets if needed
    if (init_sockets() != 0) {
        print_error("Failed to initialize sockets");
        return 1;
    }

    printf("Running defer tests...\n\n");

    // Basic functionality tests
    test_basic();
    printf("\n");

    test_nested();
    printf("\n");

    test_early_return();
    printf("\n");

    test_multiple_defers();
    printf("\n");

    // Resource management tests
    test_basic_memory();
    printf("\n");

    test_resource_cleanup();
    printf("\n");

    test_error_handling();
    printf("\n");

    // Memory tests
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

    // Real-world usage examples
    test_database_connection();
    printf("\n");

    test_mutex_locking();
    printf("\n");

    test_socket_handling();
    printf("\n");

    test_opengl_resources();
    printf("\n");

    printf("All tests completed.\n");
    return 0;
} 