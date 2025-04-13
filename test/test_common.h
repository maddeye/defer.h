/**
 * @file test_common.h
 * @brief Common declarations and utilities for defer.h tests
 */

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#define socket_t SOCKET
#define INVALID_SOCKET_VALUE INVALID_SOCKET
#define PATH_SEP '\\'
#define close_socket closesocket
#define sleep(x) Sleep((x) * 1000)
typedef CRITICAL_SECTION mutex_t;
#define MUTEX_INITIALIZER {0}
static inline int mutex_init(mutex_t* mutex) {
    InitializeCriticalSection(mutex);
    return 1;
}
static inline int mutex_lock(mutex_t* mutex) {
    EnterCriticalSection(mutex);
    return 1;
}
static inline int mutex_unlock(mutex_t* mutex) {
    LeaveCriticalSection(mutex);
    return 1;
}
static inline void mutex_destroy(mutex_t* mutex) {
    DeleteCriticalSection(mutex);
}
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define socket_t int
#define INVALID_SOCKET_VALUE -1
#define PATH_SEP '/'
#define close_socket close
typedef pthread_mutex_t mutex_t;
#define MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
static inline int mutex_init(mutex_t* mutex) {
    return pthread_mutex_init(mutex, NULL) == 0;
}
static inline int mutex_lock(mutex_t* mutex) {
    return pthread_mutex_lock(mutex) == 0;
}
static inline int mutex_unlock(mutex_t* mutex) {
    return pthread_mutex_unlock(mutex) == 0;
}
static inline void mutex_destroy(mutex_t* mutex) {
    pthread_mutex_destroy(mutex);
}
#endif

// Include defer.h after all other includes and definitions
#include "../defer.h"

// Test function declarations
void test_basic(void);
void test_nested(void);
void test_early_return(void);
void test_file_handling(void);
void test_error_handling(void);
void test_file_open_failure(void);
void test_socket_handling(void);
void test_basic_memory(void);
void test_basic_file(void);
void test_basic_string(void);
void test_multiple_defers(void);
void test_resource_cleanup(void);
void test_string_operations(void);
void test_array_operations(void);
void test_nested_resources(void);
void test_zero_allocation(void);
void test_large_allocation(void);
void test_multiple_allocations(void);
void test_aligned_allocation(void);
void test_reallocation(void);
void test_custom_cleanup(void);
void test_allocation_errors(void);
void test_nested_scope_allocation(void);
void test_database_connection(void);
void test_mutex_locking(void);
void test_opengl_resources(void);

// Utility function declarations
void print_error(const char* message);
void print_success(const char* message);
int remove_wrapper(void* path);
void socket_cleanup(void* sock_ptr);
int init_sockets(void);

#ifdef __cplusplus
}
#endif

#endif // TEST_COMMON_H 