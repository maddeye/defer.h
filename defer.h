/**
 * @file defer.h
 * @brief A single-header implementation of defer functionality for C/C++
 * 
 * This library provides a mechanism to execute cleanup code at the end of the current scope.
 * It is particularly useful for resource management, ensuring that resources are properly
 * cleaned up even if the function exits early or encounters an error.
 * 
 * # Usage
 * 
 * To use this library, define `DEFER_IMPLEMENTATION` in exactly one source file before including
 * the header. In all other source files, simply include the header without the definition.
 * 
 * ```c
 * // In one source file:
 * #define DEFER_IMPLEMENTATION
 * #include "defer.h"
 * 
 * // In other source files:
 * #include "defer.h"
 * ```
 * 
 * # Examples
 * 
 * ## Basic Usage
 * 
 * ```c
 * void cleanup(void* arg) {
 *     printf("Cleaning up: %s\n", (char*)arg);
 * }
 * 
 * void example() {
 *     char* str = "Hello";
 *     defer(cleanup, str);
 *     // str will be cleaned up when example() returns
 * }
 * ```
 * 
 * ## Resource Management
 * 
 * ```c
 * void example() {
 *     // Memory allocation with automatic cleanup
 *     int* array = malloc(100 * sizeof(int));
 *     defer_free(array);
 * 
 *     // File handling with automatic cleanup
 *     FILE* file = fopen("example.txt", "r");
 *     defer_fclose(file);
 * 
 *     // Multiple resources in the same scope
 *     FILE* log = fopen("log.txt", "w");
 *     defer_fclose(log);
 * 
 *     int* buffer = malloc(1024);
 *     defer_free(buffer);
 * }
 * ```
 * 
 * ## Error Handling
 * 
 * ```c
 * int process_file(const char* filename) {
 *     FILE* file = fopen(filename, "r");
 *     if (!file) return -1;
 *     defer_fclose(file);
 * 
 *     // Process file...
 *     // If any error occurs, the file will still be closed
 *     return 0;
 * }
 * ```
 * 
 * # Configuration
 * 
 * - `DEFER_IMPLEMENTATION`: Define in one source file to get the implementation
 */

#ifndef DEFER_H
#define DEFER_H

#include <stdlib.h>

#ifndef DEFER_FREE
#define DEFER_FREE free
#endif

#ifndef DEFER_FCLOSE
#define DEFER_FCLOSE fclose
#endif

// Internal structure to hold the deferred function and its argument
typedef struct {
    void (*func)(void*);
    void* arg;
} defer_data_t;

// Function declarations
void cleanup_free(void* ptr);
void cleanup_fclose(void* ptr);
void defer_cleanup(defer_data_t* data);

#define DEFER_CONCAT_(a, b) a##b
#define DEFER_CONCAT(a, b) DEFER_CONCAT_(a, b)

// MSVC-compatible implementation
#ifdef _MSC_VER
    // Stack to hold cleanup functions
    static defer_data_t* defer_stack = NULL;
    static size_t defer_stack_size = 0;
    static size_t defer_stack_capacity = 0;

    // Function to push cleanup function onto stack
    static void defer_push(defer_data_t data) {
        if (defer_stack_size >= defer_stack_capacity) {
            size_t new_capacity = defer_stack_capacity ? defer_stack_capacity * 2 : 16;
            defer_data_t* new_stack = (defer_data_t*)realloc(defer_stack, new_capacity * sizeof(defer_data_t));
            if (new_stack) {
                defer_stack = new_stack;
                defer_stack_capacity = new_capacity;
            }
        }
        if (defer_stack_size < defer_stack_capacity) {
            defer_stack[defer_stack_size++] = data;
        }
    }

    // Function to pop and execute cleanup function
    static void defer_pop(void) {
        if (defer_stack_size > 0) {
            defer_data_t data = defer_stack[--defer_stack_size];
            if (data.func && data.arg) {
                data.func(data.arg);
            }
        }
    }

    #define defer(func, arg) \
        defer_push((defer_data_t){ (void (*)(void*))func, (void*)(arg) }); \
        atexit(defer_pop)

    #define defer_free(ptr) \
        void* DEFER_CONCAT(__defer_ptr_, __LINE__) = (void*)(ptr); \
        defer_push((defer_data_t){ cleanup_free, DEFER_CONCAT(__defer_ptr_, __LINE__) }); \
        atexit(defer_pop)

    #define defer_fclose(fp) \
        FILE* DEFER_CONCAT(__defer_fp_, __LINE__) = (FILE*)(fp); \
        defer_push((defer_data_t){ cleanup_fclose, DEFER_CONCAT(__defer_fp_, __LINE__) }); \
        atexit(defer_pop)
#else

    #define defer(func, arg) \
        __attribute__((cleanup(defer_cleanup))) \
        defer_data_t DEFER_CONCAT(__defer_data_, __LINE__) = { (void (*)(void*))func, arg }

    #define defer_free(ptr) defer(cleanup_free, ptr)
    #define defer_fclose(fp) defer(cleanup_fclose, fp)
#endif

#ifdef DEFER_IMPLEMENTATION

// Function implementations
void cleanup_free(void* ptr) {
    if (ptr) {
        printf("Cleaning up free: %p\n", ptr);
        DEFER_FREE(ptr);
    }
}

void cleanup_fclose(void* ptr) {
    if (ptr) {
        DEFER_FCLOSE((FILE*)ptr);
    }
}

void defer_cleanup(defer_data_t* data) {
    if (data && data->func && data->arg) {
        data->func(data->arg);
    }
}

#endif // DEFER_IMPLEMENTATION

#endif // DEFER_H