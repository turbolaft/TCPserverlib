# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g
# Source directory
SRC_DIR = src
# Object directory
OBJ_DIR = bin/objs
# Executable name
EXE = bin/main

# List of source files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
# List of object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Main bin
$(EXE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJ_FILES)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(EXE)