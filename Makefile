# Default compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -I.

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    # Windows settings
    CC = gcc
    CFLAGS += -D_WIN32_WINNT=0x0601
    LDFLAGS = -lws2_32
    EXE_EXT = .exe
else
    # Unix-like settings
    LDFLAGS = -lpthread
    EXE_EXT =
endif

# Test sources
TEST_SOURCES = test/test_defer.c test/test_common.c test/test_memory.c test/test_files.c test/test_cases.c test/test_basic.c test/test_resources.c

# Example sources
EXAMPLE_SOURCES = example/file_example.c example/socket_example.c example/resource_example.c

# Test targets
TEST_TARGETS = defer_test_gcc defer_test_clang

# Example targets
EXAMPLE_TARGETS = file_example socket_example resource_example

# Default target
all: $(TEST_TARGETS) $(EXAMPLE_TARGETS)

# Clean target
clean:
	rm -f $(TEST_TARGETS) $(EXAMPLE_TARGETS) *.o

# Test targets
defer_test_gcc: $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

defer_test_clang: $(TEST_SOURCES)
	clang $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Example targets
file_example: example/file_example.c
	$(CC) $(CFLAGS) -o $@ $<

socket_example: example/socket_example.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

resource_example: example/resource_example.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Test running targets
test: all
	./defer_test_gcc
	./defer_test_clang

test_gcc: defer_test_gcc
	./defer_test_gcc

test_clang: defer_test_clang
	./defer_test_clang

# Example running targets
examples: $(EXAMPLE_TARGETS)
	./file_example
	./socket_example
	./resource_example

.PHONY: all clean test test_gcc test_clang examples 