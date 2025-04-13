#define DEFER_IMPLEMENTATION
#include "test_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
static WSADATA wsaData;

int init_sockets(void) {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        print_error("WSAStartup failed");
        return -1;
    }
    return 0;
}

void cleanup_sockets(void) {
    WSACleanup();
}
#else
int init_sockets(void) {
    return 0; // No initialization needed on Unix-like systems
}
#endif

void print_error(const char* message) {
    printf("ERROR: %s\n", message);
}

void print_success(const char* message) {
    printf("SUCCESS: %s\n", message);
}

int remove_wrapper(void* path) {
    return remove((const char*)path);
}

void socket_cleanup(void* sock_ptr) {
    socket_t* sock = (socket_t*)sock_ptr;
    if (*sock != INVALID_SOCKET_VALUE) {
        close_socket(*sock);
        *sock = INVALID_SOCKET_VALUE;
    }
} 