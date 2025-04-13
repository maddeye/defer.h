/**
 * @file file_example.c
 * @brief Example of using defer for file handling
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFER_IMPLEMENTATION
#include "../defer.h"

int main(void) {
    // Example 1: Basic file operations
    printf("Example 1: Basic file operations\n");
    {
        FILE* file = fopen("example.txt", "w");
        if (!file) {
            printf("Failed to open file\n");
            return 1;
        }
        defer_fclose(file);

        fprintf(file, "Hello, World!\n");
        printf("File written successfully\n");
    }

    // Example 2: Multiple files with error handling
    printf("\nExample 2: Multiple files with error handling\n");
    {
        FILE* source = fopen("example.txt", "r");
        if (!source) {
            printf("Failed to open source file\n");
            return 1;
        }
        defer_fclose(source);

        FILE* dest = fopen("example_copy.txt", "w");
        if (!dest) {
            printf("Failed to open destination file\n");
            return 1;
        }
        defer_fclose(dest);

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), source)) {
            fprintf(dest, "%s", buffer);
        }
        printf("File copied successfully\n");
    }

    // Example 3: Temporary file cleanup
    printf("\nExample 3: Temporary file cleanup\n");
    {
        FILE* temp = fopen("temp_data.tmp", "w");
        if (!temp) {
            printf("Failed to create temporary file\n");
            return 1;
        }
        defer_fclose(temp);

        // Write some temporary data
        fprintf(temp, "Temporary data\n");
        printf("Temporary file created and will be automatically closed\n");

        // The file will be closed when the scope ends
    }

    return 0;
} 