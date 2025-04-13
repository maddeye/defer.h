/**
 * @file test_cases.c
 * @brief Implementation of test cases for defer.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "test_common.h"
#include "../defer.h"

void test_basic(void) {
    int* data = malloc(sizeof(int));
    if (!data) {
        print_error("Failed to allocate memory");
        return;
    }
    defer_free(data);

    *data = 42;
    print_success("Basic test completed");
}

void test_nested(void) {
    char* outer = strdup("outer");
    if (!outer) {
        print_error("Failed to allocate outer string");
        return;
    }
    defer_free(outer);

    char* inner = strdup("inner");
    if (!inner) {
        print_error("Failed to allocate inner string");
        return;
    }
    defer_free(inner);

    print_success("Nested test completed");
}

void test_early_return(void) {
    char path[256];
    snprintf(path, sizeof(path), "build%ctest.txt", PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to open file");
        return;
    }
    defer_fclose(file);

    // Early return with cleanup
    fprintf(file, "Early return test\n");
    print_success("Early return test completed");
    return;

    // This code will never execute, but file will still be closed
    fprintf(file, "This should not be written\n");
}

void test_file_handling(void) {
    char path[256];
    snprintf(path, sizeof(path), "build%ctest.txt", PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to open file");
        return;
    }
    defer_fclose(file);

    // Write some data
    fprintf(file, "Hello, World!\n");
    fflush(file);

    // File will be automatically closed when the function returns
}

void test_error_handling(void) {
    errno = EINVAL;
    char* data = malloc(100);
    if (!data) {
        print_error("Failed to allocate memory");
        return;
    }
    defer_free(data);

    if (errno != 0) {
        print_error("Error occurred during allocation");
        return;
    }

    print_success("Error handling test completed");
}

void test_file_open_failure(void) {
    char path[256];
    snprintf(path, sizeof(path), "nonexistent_directory%cbuild%ctest.txt", PATH_SEP, PATH_SEP);
    FILE* file = fopen(path, "w");
    if (!file) {
        print_error("Failed to open file (expected)");
        return;
    }
    defer_fclose(file);

    // This code won't be reached if the file open fails
}

void test_socket_handling(void) {
    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET_VALUE) {
        print_error("Failed to create socket");
        return;
    }
    defer(socket_cleanup, &sock);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        print_error("Failed to bind socket");
        return;
    }

    print_success("Socket handling test completed");
}

// Mock database connection structure
typedef struct {
    int connected;
    char* connection_string;
} db_connection_t;

// Mock database cleanup function
static void db_cleanup(void* arg) {
    db_connection_t* conn = (db_connection_t*)arg;
    if (conn) {
        printf("Closing database connection: %s\n", conn->connection_string);
        free(conn->connection_string);
        conn->connected = 0;
    }
}

void test_database_connection(void) {
    db_connection_t conn = {0};
    conn.connection_string = strdup("postgresql://localhost:5432/testdb");
    if (!conn.connection_string) {
        print_error("Failed to allocate connection string");
        return;
    }
    defer(db_cleanup, &conn);
    
    conn.connected = 1;
    printf("Connected to database: %s\n", conn.connection_string);
    print_success("Database connection test completed");
}

// Mock mutex structure
typedef struct {
    pthread_mutex_t mutex;
    int locked;
} custom_mutex_t;

// Mock mutex cleanup function
static void mutex_cleanup(void* arg) {
    custom_mutex_t* mtx = (custom_mutex_t*)arg;
    if (mtx && mtx->locked) {
        pthread_mutex_unlock(&mtx->mutex);
        mtx->locked = 0;
        printf("Mutex unlocked\n");
    }
}

void test_mutex_locking(void) {
    custom_mutex_t mtx = {0};
    if (pthread_mutex_init(&mtx.mutex, NULL) != 0) {
        print_error("Failed to initialize mutex");
        return;
    }
    defer(mutex_cleanup, &mtx);
    
    if (pthread_mutex_lock(&mtx.mutex) != 0) {
        print_error("Failed to lock mutex");
        return;
    }
    mtx.locked = 1;
    printf("Mutex locked\n");
    
    // Simulate some work
    printf("Performing work with mutex locked\n");
    
    print_success("Mutex locking test completed");
}

// Mock OpenGL resource structure
typedef struct {
    unsigned int texture_id;
    int bound;
} gl_texture_t;

// Mock OpenGL cleanup function
static void gl_cleanup(void* arg) {
    gl_texture_t* tex = (gl_texture_t*)arg;
    if (tex && tex->bound) {
        printf("Unbinding texture %u\n", tex->texture_id);
        tex->bound = 0;
    }
}

void test_opengl_resources(void) {
    gl_texture_t tex = {0};
    tex.texture_id = 12345; // Mock texture ID
    defer(gl_cleanup, &tex);
    
    printf("Binding texture %u\n", tex.texture_id);
    tex.bound = 1;
    
    // Simulate some OpenGL operations
    printf("Using texture %u for rendering\n", tex.texture_id);
    
    print_success("OpenGL resource management test completed");
} 