# Default compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -I.

# Build directory
BUILD_DIR = build

# Platform-specific settings
ifeq ($(OS),Windows_NT)
   # Windows settings
    CC = gcc
    CFLAGS += -D_WIN32_WINNT=0x0601
    LDFLAGS = -lws2_32
    EXE_EXT = .exe
    # MSVC settings
    MSVC = cl
    MSVC_CFLAGS = /W4 /EHsc /I. /D_WIN32_WINNT=0x0601
    # MSVC test sources
    MSVC_TEST_SOURCES = test/test_msvc.c test/test_common.c
else
    # Unix-like settings
    LDFLAGS = -lpthread
    EXE_EXT =
    # macOS specific settings
    ifeq ($(shell uname),Darwin)
        CC = gcc-13
        CLANG = clang
        CFLAGS += -arch $(shell uname -m)
    else
        CC = gcc
        CLANG = clang
    endif
endif

# Test sources
TEST_SOURCES = test/test_defer.c test/test_common.c test/test_memory.c test/test_files.c test/test_cases.c test/test_basic.c test/test_resources.c

# Example sources
EXAMPLE_SOURCES = example/file_example.c example/socket_example.c example/resource_example.c

# Test targets
TEST_TARGETS = defer_test_gcc defer_test_clang
ifeq ($(OS),Windows_NT)
    TEST_TARGETS += defer_test_msvc
endif


# Example sources
EXAMPLE_SOURCES = example/file_example.c example/socket_example.c example/resource_example.c

# Test targets
TEST_TARGETS = $(BUILD_DIR)/defer_test_gcc $(BUILD_DIR)/defer_test_clang
ifeq ($(OS),Windows_NT)
    TEST_TARGETS += $(BUILD_DIR)/defer_test_msvc
endif

# Example targets
EXAMPLE_TARGETS = $(BUILD_DIR)/file_example $(BUILD_DIR)/socket_example $(BUILD_DIR)/resource_example

# Default target
all: $(BUILD_DIR) $(TEST_TARGETS) $(EXAMPLE_TARGETS)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR)

# Test targets
$(BUILD_DIR)/defer_test_gcc: $(TEST_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/defer_test_clang: $(TEST_SOURCES) | $(BUILD_DIR)
	$(CLANG) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/defer_test_msvc: $(MSVC_TEST_SOURCES) | $(BUILD_DIR)
	$(MSVC) $(MSVC_CFLAGS) /Fe$@ $^ ws2_32.lib

# Example targets
$(BUILD_DIR)/file_example: example/file_example.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/socket_example: example/socket_example.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/resource_example: example/resource_example.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Test running targets
test: all
ifeq ($(OS),Windows_NT)
	$(BUILD_DIR)/defer_test_msvc
else
	$(BUILD_DIR)/defer_test_gcc
	$(BUILD_DIR)/defer_test_clang
endif

test_gcc: $(BUILD_DIR)/defer_test_gcc
	$(BUILD_DIR)/defer_test_gcc

test_clang: $(BUILD_DIR)/defer_test_clang
	$(BUILD_DIR)/defer_test_clang

test_msvc: $(BUILD_DIR)/defer_test_msvc
	$(BUILD_DIR)/defer_test_msvc

# Valgrind target
valgrind: $(BUILD_DIR)/defer_test_gcc
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $(BUILD_DIR)/defer_test_gcc

# Example running targets
examples: $(EXAMPLE_TARGETS)
	$(BUILD_DIR)/file_example
	$(BUILD_DIR)/socket_example
	$(BUILD_DIR)/resource_example

.PHONY: all clean test test_gcc test_clang test_msvc valgrind examples 