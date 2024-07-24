# Compiler and flags
CXX = g++
CXXFLAGS = -fPIC -Wall -std=c++17
LDFLAGS = -shared

# Directories
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin

# Files
LIB_SRC = $(SRC_DIR)/source.cpp
LIB_NAME = libsrc.so
LIB = $(LIB_DIR)/$(LIB_NAME)
TEST_SRC = $(TEST_DIR)/main.cpp
TEST_EXEC = $(BIN_DIR)/test_exec

# Default target
all: build

# Build target
build: $(LIB)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB): $(LIB_SRC) | $(LIB_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(LIB) $(LIB_SRC)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_EXEC): $(TEST_SRC) $(LIB) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_SRC) -L./$(LIB_DIR) -lsrc -I./$(SRC_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR)		

# Run target
run: $(TEST_EXEC)
	 LD_LIBRARY_PATH=$(LIB_DIR) ./$(TEST_EXEC) $(ARGS)

# Test target
test: $(TEST_EXEC)
	 LD_LIBRARY_PATH=$(LIB_DIR) ./$(TEST_EXEC) $(ARGS)

.PHONY: all build clean test run
