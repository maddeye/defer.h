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
    # MSVC settings
    MSVC = cl
    MSVC_CFLAGS = /W4 /EHsc /I.
else
    # Unix-like settings
    LDFLAGS = -lpthread
    EXE_EXT =
endif

# Test sources
TEST_SOURCES = test/test_defer.c test/test_common.c test/test_memory.c test/test_files.c test/test_cases.c test/test_basic.c test/test_resources.c test/test_msvc.c

# Example sources
EXAMPLE_SOURCES = example/file_example.c example/socket_example.c example/resource_example.c

# Test targets
TEST_TARGETS = defer_test_gcc defer_test_clang
ifeq ($(OS),Windows_NT)
    TEST_TARGETS += defer_test_msvc
endif

# Example targets
EXAMPLE_TARGETS = file_example socket_example resource_example

# Default target
all: $(TEST_TARGETS) $(EXAMPLE_TARGETS)

# Clean target
clean:
	rm -f $(TEST_TARGETS) $(EXAMPLE_TARGETS) *.o *.obj

# Test targets
defer_test_gcc: $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

defer_test_clang: $(TEST_SOURCES)
	clang $(CFLAGS) -o $@ $^ $(LDFLAGS)

defer_test_msvc: $(TEST_SOURCES)
	$(MSVC) $(MSVC_CFLAGS) /Fe$@ $^ ws2_32.lib

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
ifeq ($(OS),Windows_NT)
	./defer_test_msvc
endif

test_gcc: defer_test_gcc
	./defer_test_gcc

test_clang: defer_test_clang
	./defer_test_clang

test_msvc: defer_test_msvc
	./defer_test_msvc

# Valgrind target
valgrind: defer_test_gcc
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./defer_test_gcc

# Example running targets
examples: $(EXAMPLE_TARGETS)
	./file_example
	./socket_example
	./resource_example

.PHONY: all clean test test_gcc test_clang test_msvc valgrind examples 