# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Directories
SRC_DIR = .
OBJ_DIR = obj

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/utility_functions.cpp \
       $(SRC_DIR)/error_handler.cpp

# Object files
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/utility_functions.o \
       $(OBJ_DIR)/error_handler.o

# Header files
HDRS = $(SRC_DIR)/config_structures.h \
       $(SRC_DIR)/utility_functions.h \
       $(SRC_DIR)/error_handler.h

# Target executable
TARGET = main

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Default rule
all: $(OBJ_DIR) $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
