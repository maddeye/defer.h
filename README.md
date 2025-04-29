# defer.h

> [!WARNING]
> This library is not compatible with MSVC right now.
> If you wanna help, open a PR.

A single-header C library that implements the defer statement pattern, similar to Go's `defer` statement. This allows you to schedule cleanup code to run when the current scope exits.

## Features

- Single header file implementation
- No dependencies
- Cross-platform (Windows, Linux, macOS)
- Works with GCC and Clang
- Thread-safe
- Supports nested scopes
- Automatic resource cleanup

## Quick Start

1. Copy `defer.h` into your project
2. Include the header in your source file
3. Use `defer` or convenience macros to schedule cleanup

```c
#include <stdio.h>

#define DEFER_IMPLEMENTATION
#include "defer.h"

// Simple example: Print a message when function exits
void print_message(void* arg) {
    printf("Cleanup: %s\n", (char*)arg);
}

int main() {
    char* message = "Function is ending";
    defer(print_message, message);
    printf("Function is running...\n");
    return 0;
}
```

```c
#include <stdio.h>

#define DEFER_IMPLEMENTATION
#include "defer.h"

int main() {
    FILE* file = fopen("example.txt", "w");
    if (!file) return 1;
    defer_fclose(file);  // File will be closed when main() exits

    // Work with file...
    fprintf(file, "Hello, World!\n");
    return 0;
}
```

## Usage Examples

### Memory Management
```c
void* data = malloc(1024);
if (!data) return;
defer_free(data);  // Memory will be freed when function returns

// Use data...
memset(data, 0, 1024);
```

### Multiple Resources
```c
{
    FILE* log = fopen("log.txt", "w");
    if (!log) return;
    defer_fclose(log);

    void* buffer = malloc(1024);
    if (!buffer) return;
    defer_free(buffer);

    // Both log and buffer will be cleaned up
    // in reverse order when scope ends
}
```

### Custom Cleanup
```c
typedef struct {
    int connected;
    char* url;
} Connection;

void cleanup_connection(void* arg) {
    Connection* conn = (Connection*)arg;
    if (conn && conn->connected) {
        printf("Disconnecting from %s\n", conn->url);
        conn->connected = 0;
    }
}

void example() {
    Connection conn = {1, "example.com"};
    defer(cleanup_connection, &conn);
    
    // Connection will be cleaned up on any return
    if (error_condition) return;
    // ... normal execution ...
}
```

## Test Coverage

The library has been extensively tested with the following scenarios:

1. Basic Functionality
   - `test_basic()`: Basic defer operation
   - `test_nested()`: Nested defer statements
   - `test_early_return()`: Early function returns
   - `test_multiple_defers()`: Multiple defers in same scope

2. Memory Management
   - `test_zero_allocation()`: Zero-size allocations
   - `test_large_allocation()`: Large memory blocks
   - `test_multiple_allocations()`: Multiple memory allocations
   - `test_aligned_allocation()`: Aligned memory allocation
   - `test_reallocation()`: Memory reallocation
   - `test_nested_scope_allocation()`: Nested scope memory management

3. Resource Management
   - `test_file_operations()`: File handling
   - `test_socket_handling()`: Network sockets
   - `test_mutex_locking()`: Thread synchronization
   - `test_database_connection()`: Database connections
   - `test_opengl_resources()`: Graphics resources

4. Error Handling
   - `test_allocation_errors()`: Memory allocation failures
   - `test_file_open_failure()`: File operation failures
   - `test_error_handling()`: General error conditions

## Building and Testing

```bash
# Build all tests and examples
make all

# Run all tests
make test

# Run specific test
make test_gcc    # Run GCC tests
make test_clang  # Run Clang tests

# Build and run examples
make examples
```

## Example Programs

The `example` directory contains complete programs demonstrating real-world usage:

1. `file_example.c`: File handling patterns
2. `socket_example.c`: Network programming
3. `resource_example.c`: Resource management patterns

## License

This library is released under the MIT License. See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.