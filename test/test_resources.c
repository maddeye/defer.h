/**
 * @file test_resources.c
 * @brief Resource management tests for defer.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_common.h"
#include "../defer.h"

// Resource management example
typedef struct {
    char* data;
    size_t size;
} Resource;

static void free_resource(void* ptr) {
    Resource** res = (Resource**)ptr;
    if (res && *res) {
        free(*res);
        printf("Resource freed\n");
    }
}

// Memory safety test structures
typedef struct {
    int* array;
    size_t size;
} ArrayResource;

static void free_array(void* ptr) {
    ArrayResource* arr = (ArrayResource*)ptr;
    if (arr && arr->array) {
        free(arr->array);
        printf("Array of size %lu freed\n", (unsigned long)arr->size);
    }
}

// Test resource management
void test_resource_management(void) {
    printf("\n=== Testing Resource Management ===\n");
    printf("Allocating resource\n");
    
    Resource* res = (Resource*)malloc(sizeof(Resource));
    if (!res) {
        print_error("Allocating resource");
        return;
    }
    
    defer(free_resource, &res);
    res->data = strdup("test");
    if (!res->data) {
        print_error("Failed to allocate string");
        return;
    }
    printf("Resource used: %s\n", res->data);
}

// Test memory safety with array allocation
void test_array_allocation(void) {
    printf("\n=== Testing Array Allocation ===\n");
    
    ArrayResource arr = {0};
    arr.size = 10;
    arr.array = (int*)malloc(arr.size * sizeof(int));
    if (!arr.array) {
        print_error("Allocating array");
        return;
    }
    
    defer(free_array, &arr);
    
    // Initialize array
    for (size_t i = 0; i < arr.size; i++) {
        arr.array[i] = (int)i;
    }
    
    printf("Array initialized with values 0 to %lu\n", (unsigned long)(arr.size - 1));
}

void test_file_operations(void) {
    char path[256];
    snprintf(path, sizeof(path), "test%ctest.txt", PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to open file for writing");
        return;
    }
    defer_fclose(file);

    fprintf(file, "Test content\n");
    print_success("File operations test completed");
}

void test_memory_operations(void) {
    void* ptr = malloc(100);
    if (!ptr) {
        print_error("Memory allocation failed");
        return;
    }
    defer_free(ptr);

    memset(ptr, 0, 100);
    print_success("Memory operations test completed");
}

void test_resource_cleanup(void) {
    // Test file cleanup
    char path[256];
    snprintf(path, sizeof(path), "build%ctemp_data.tmp", PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to open temporary file");
        return;
    }
    defer_fclose(file);

    // Test memory cleanup
    void* ptr = malloc(200);
    if (!ptr) {
        print_error("Memory allocation failed");
        return;
    }
    defer_free(ptr);

    // Test socket cleanup
    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET_VALUE) {
        print_error("Socket creation failed");
        return;
    }
    defer(socket_cleanup, &sock);

    print_success("Resource cleanup test completed");
}

void test_temp_resource_cleanup(void) {
    char path[256];
    snprintf(path, sizeof(path), "build%ctemp_data.tmp", PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to create temporary file");
        return;
    }
    defer_fclose(file);

    // Test memory cleanup
    void* ptr = malloc(200);
    if (!ptr) {
        print_error("Memory allocation failed");
        return;
    }
    defer_free(ptr);

    // Test socket cleanup
    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET_VALUE) {
        print_error("Socket creation failed");
        return;
    }
    defer(socket_cleanup, &sock);

    print_success("Temporary resource cleanup test completed");
}

void test_string_operations(void) {
    char* str = strdup("Hello, World!");
    if (!str) {
        print_error("Failed to allocate string");
        return;
    }
    defer_free(str);

    // Perform string operations
    size_t len = strlen(str);
    if (len == 0) {
        print_error("String is empty");
        return;
    }

    print_success("String operations test completed");
}

void test_array_operations(void) {
    size_t size = 100;
    int* array = malloc(size * sizeof(int));
    if (!array) {
        print_error("Failed to allocate array");
        return;
    }
    defer_free(array);

    // Initialize array
    for (size_t i = 0; i < size; i++) {
        array[i] = (int)i;
    }

    print_success("Array operations test completed");
}

void test_nested_resources(void) {
    // Outer resource
    void* outer = malloc(100);
    if (!outer) {
        print_error("Failed to allocate outer resource");
        return;
    }
    defer_free(outer);

    // Inner resource
    void* inner = malloc(50);
    if (!inner) {
        print_error("Failed to allocate inner resource");
        return;
    }
    defer_free(inner);

    // Nested file operation
    char path[256];
    snprintf(path, sizeof(path), "build%ctest.txt", PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to open file");
        return;
    }
    defer_fclose(file);

    print_success("Nested resources test completed");
}

void test_nested_resource_cleanup(void) {
    char path[256];
    snprintf(path, sizeof(path), "build%ctest.txt", PATH_SEP);
    FILE* outer = fopen(path, "w");
    if (!outer) {
        print_error("Failed to open outer file");
        return;
    }
    defer_fclose(outer);

    // ... existing code ...
} 