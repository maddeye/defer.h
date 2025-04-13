/**
 * @file socket_example.c
 * @brief Example of using defer for socket handling
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defer.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define socket_t SOCKET
#define INVALID_SOCKET_VALUE INVALID_SOCKET
#define close_socket closesocket
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define socket_t int
#define INVALID_SOCKET_VALUE -1
#define close_socket close
#endif

// Socket cleanup function
static void socket_cleanup(void* sock_ptr) {
    socket_t* sock = (socket_t*)sock_ptr;
    if (*sock != INVALID_SOCKET_VALUE) {
        close_socket(*sock);
        *sock = INVALID_SOCKET_VALUE;
    }
}

int main(void) {
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }
    defer(WSACleanup, NULL);
#endif

    // Example 1: Basic socket creation and cleanup
    printf("Example 1: Basic socket operations\n");
    {
        socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET_VALUE) {
            printf("Failed to create socket\n");
            return 1;
        }
        defer(socket_cleanup, &sock);

        printf("Socket created successfully\n");
        // Socket will be closed when the scope ends
    }

    // Example 2: Socket with address binding
    printf("\nExample 2: Socket with address binding\n");
    {
        socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET_VALUE) {
            printf("Failed to create socket\n");
            return 1;
        }
        defer(socket_cleanup, &sock);

        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            printf("Failed to bind socket\n");
            return 1;
        }

        printf("Socket bound to port 8080\n");
        // Socket will be closed when the scope ends
    }

    // Example 3: Multiple sockets
    printf("\nExample 3: Multiple sockets\n");
    {
        socket_t server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == INVALID_SOCKET_VALUE) {
            printf("Failed to create server socket\n");
            return 1;
        }
        defer(socket_cleanup, &server);

        socket_t client = socket(AF_INET, SOCK_STREAM, 0);
        if (client == INVALID_SOCKET_VALUE) {
            printf("Failed to create client socket\n");
            return 1;
        }
        defer(socket_cleanup, &client);

        printf("Server and client sockets created\n");
        // Both sockets will be closed when the scope ends
    }

    return 0;
} 