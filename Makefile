# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g
# Source directory
SRC_DIR = src
# Object directory
OBJ_DIR = bin/objs
# Library directory
LIB_DIR = bin/libs
# Library name
LIB = $(LIB_DIR)/libserver.so

# List of source files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
# List of object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Main library
$(LIB): $(LIB_DIR) $(OBJ_DIR) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -shared -o $(LIB) $(OBJ_FILES)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<

# Create library directory
$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

.PHONY: clean
