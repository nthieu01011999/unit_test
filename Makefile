CC          = gcc
CXX         = g++
EXEC        = ./
CXXFLAGS    = -Wall -Wextra -std=c++17 -I/usr/include/openssl

LDLIBS      += -lpthread -lcrypto -lssl

INC         = .
SRCS        = $(INC)/main.cpp 
#             $(INC)/setNetworkConfig.cpp

OBJS        = $(SRCS:.cpp=.o)

TARGET      = main

INCLUDES    = -I$(INC)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: run
run: $(TARGET)
	@sudo $(EXEC)$(TARGET)












# Makefile for XOR File Encryption Program

# # Compiler
# CC = gcc

# # Compiler Flags
# CFLAGS = -Wall -Wextra -pedantic -std=c11

# # Executable name
# TARGET = xor_encrypt

# # Source files
# SRCS = main.c

# # Object files
# OBJS = $(SRCS:.c=.o)

# # Default rule
# all: $(TARGET)

# # Rule to build the executable
# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# # Rule to build object files
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Rule to clean up
# clean:
# 	rm -f $(OBJS) $(TARGET)

# # Rule to run the program
# run: $(TARGET)
# 	./$(TARGET) inputfile.txt password123

# .PHONY: all clean run



# # Cross-compilation setup
# CROSS_COMPILE := /home/it/Desktop/Camera/cam/rts39xx_sdk_v5.0.1/toolchain/tool/bin/arm-linux-uclibcgnueabi-
# CC          := $(CROSS_COMPILE)gcc
# CXX         := $(CROSS_COMPILE)g++
# CXXFLAGS    := -Wall -Wextra -std=c++17 -O2
# LDFLAGS     := -lpthread

# # Project structure
# INC_DIR     := include
# BUILD_DIR   := build
# TARGET_NAME := my_application
# TARGET      := $(BUILD_DIR)/$(TARGET_NAME)

# # Manually specified source files
# INC         := .
# SRCS        := 	$(INC)/main.cpp \
# 				# $(INC)/utilitiesd.cpp \
# 				# $(INC)/recorder.cpp \
# 				# $(INC)/SDCard.cpp

# # Convert source file paths to object file paths
# OBJS        := $(patsubst $(INC)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# # Default build target
# .PHONY: all
# all: print-vars create_build_dir $(TARGET)

# # Print out SRCS and OBJS for debugging
# .PHONY: print-vars
# print-vars:
# 	@echo "SRCS: $(SRCS)"
# 	@echo "OBJS: $(OBJS)"

# # Ensure build directory exists
# create_build_dir:
# 	@echo "Creating build directory..."
# 	@mkdir -p $(BUILD_DIR)

# # Build the application
# $(TARGET): $(OBJS)
# 	@echo "Linking $(TARGET_NAME)..."
# 	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# # Compile source files
# $(BUILD_DIR)/%.o: $(INC)/%.cpp
# 	@mkdir -p $(@D)
# 	@echo "Compiling $<..."
# 	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# # Clean up build artifacts
# .PHONY: clean
# clean:
# 	@echo "Cleaning up..."
# 	rm -rf $(BUILD_DIR)

# # Copy the executable to a target location (e.g., NFS share)
# .PHONY: copy
# copy: all
# 	@echo "Starting the copy process..."
# 	@echo "Checking if $(TARGET) is already built..."
# 	@echo "Building the target if necessary..."
# 	# Assuming 'all' does the build process
# 	@echo "Build process completed."
# 	@echo "Copying $(TARGET) to NFS share..."
# 	@sudo cp $(TARGET) /mnt/nfs_share
# 	@echo "Copy process completed."
# 	@echo "Verifying the copy..."
# 	@if [ -f /mnt/nfs_share/$(notdir $(TARGET)) ]; then \
# 		echo "$(TARGET) successfully copied to /mnt/nfs_share."; \
# 	else \
# 		echo "Failed to copy $(TARGET) to /mnt/nfs_share."; \
# 	fi
# 	@echo "Copy process finished."


