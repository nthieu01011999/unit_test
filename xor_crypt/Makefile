# Makefile for XOR Encryption/Decryption Tool

# Uncomment this line to enable release build
# RELEASE=1

# Uncomment this line to enable debug build
DEBUG=1

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Check for release or debug build
ifdef RELEASE
    CFLAGS += -O3 -DRELEASE
endif

ifdef DEBUG
    CFLAGS += -g -DDEBUG
endif

# Executable name
TARGET = xor_tool

# Directory for object files
OBJ_DIR = build

# Object files
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/xor_encrypt.o $(OBJ_DIR)/xor_decrypt.o

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to create the build directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to build main.o
$(OBJ_DIR)/main.o: main.c xor_encrypt.h xor_decrypt.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o

# Rule to build xor_encrypt.o
$(OBJ_DIR)/xor_encrypt.o: xor_encrypt.c xor_encrypt.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c xor_encrypt.c -o $(OBJ_DIR)/xor_encrypt.o

# Rule to build xor_decrypt.o
$(OBJ_DIR)/xor_decrypt.o: xor_decrypt.c xor_decrypt.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c xor_decrypt.c -o $(OBJ_DIR)/xor_decrypt.o

# Rule to clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rule to run the program
run_encrypt: $(TARGET)
	./$(TARGET) encrypt inputfile.txt password123

run_decrypt: $(TARGET)
	./$(TARGET) decrypt new-inputfile.txt password123

.PHONY: all clean run_encrypt run_decrypt
