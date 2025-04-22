# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -std=c++17

# SFML flags (adjust if SFML is in a non-standard path)
SFML_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Directories and output
SRC_DIR := MarioClone
BUILD_DIR := build
BIN := $(BUILD_DIR)/main

# Sources and objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN)

# Link objects into the final binary
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_FLAGS)

# Compile .cpp files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

