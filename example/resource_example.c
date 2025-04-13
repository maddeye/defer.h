/**
 * @file resource_example.c
 * @brief Example of using defer for resource management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFER_IMPLEMENTATION
#include "../defer.h"

// Example 1: Database connection
typedef struct {
    int connected;
    char* connection_string;
} db_connection_t;

static void db_cleanup(void* arg) {
    db_connection_t* conn = (db_connection_t*)arg;
    if (conn) {
        printf("Closing database connection: %s\n", conn->connection_string);
        free(conn->connection_string);
        conn->connected = 0;
    }
}

// Example 2: Mutex handling
#ifdef _WIN32
#include <windows.h>
typedef CRITICAL_SECTION mutex_t;
#else
#include <pthread.h>
typedef pthread_mutex_t mutex_t;
#endif

typedef struct {
    mutex_t mutex;
    int locked;
} custom_mutex_t;

static void mutex_cleanup(void* arg) {
    custom_mutex_t* mtx = (custom_mutex_t*)arg;
    if (mtx && mtx->locked) {
#ifdef _WIN32
        LeaveCriticalSection(&mtx->mutex);
#else
        pthread_mutex_unlock(&mtx->mutex);
#endif
        mtx->locked = 0;
        printf("Mutex unlocked\n");
    }
}

// Example 3: OpenGL resource
typedef struct {
    unsigned int texture_id;
    int bound;
} gl_texture_t;

static void gl_cleanup(void* arg) {
    gl_texture_t* tex = (gl_texture_t*)arg;
    if (tex && tex->bound) {
        printf("Unbinding texture %u\n", tex->texture_id);
        tex->bound = 0;
    }
}

int main(void) {
    // Example 1: Database connection
    printf("Example 1: Database connection\n");
    {
        db_connection_t conn = {0};
        conn.connection_string = strdup("postgresql://localhost:5432/testdb");
        if (!conn.connection_string) {
            printf("Failed to allocate connection string\n");
            return 1;
        }
        defer(db_cleanup, &conn);
        
        conn.connected = 1;
        printf("Connected to database: %s\n", conn.connection_string);
        // Connection will be closed when the scope ends
    }

    // Example 2: Mutex handling
    printf("\nExample 2: Mutex handling\n");
    {
        custom_mutex_t mtx = {0};
#ifdef _WIN32
        InitializeCriticalSection(&mtx.mutex);
#else
        pthread_mutex_init(&mtx.mutex, NULL);
#endif
        defer(mutex_cleanup, &mtx);
        
#ifdef _WIN32
        EnterCriticalSection(&mtx.mutex);
#else
        pthread_mutex_lock(&mtx.mutex);
#endif
        mtx.locked = 1;
        printf("Mutex locked\n");
        
        // Simulate some work
        printf("Performing work with mutex locked\n");
        // Mutex will be unlocked when the scope ends
    }

    // Example 3: OpenGL resource
    printf("\nExample 3: OpenGL resource\n");
    {
        gl_texture_t tex = {0};
        tex.texture_id = 12345; // Mock texture ID
        defer(gl_cleanup, &tex);
        
        printf("Binding texture %u\n", tex.texture_id);
        tex.bound = 1;
        
        // Simulate some OpenGL operations
        printf("Using texture %u for rendering\n", tex.texture_id);
        // Texture will be unbound when the scope ends
    }

    return 0;
} 