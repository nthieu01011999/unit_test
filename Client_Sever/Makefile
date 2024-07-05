# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Target executable
TARGET = main

# Source files
SRCS = main.cpp Client.cpp Server.cpp

# Build directory
BUILD_DIR = build

# Object files (store in build directory)
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to build object files from source files (store in build directory)
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rule to remove object files and executable
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean
