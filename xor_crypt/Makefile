# Makefile for XOR Encryption/Decryption Tool

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g

# Executable name
TARGET = xor_tool

# Object files
OBJS = main.o xor_encrypt.o xor_decrypt.o

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build main.o
main.o: main.c xor_encrypt.h xor_decrypt.h
	$(CC) $(CFLAGS) -c main.c

# Rule to build xor_encrypt.o
xor_encrypt.o: xor_encrypt.c xor_encrypt.h
	$(CC) $(CFLAGS) -c xor_encrypt.c

# Rule to build xor_decrypt.o
xor_decrypt.o: xor_decrypt.c xor_decrypt.h
	$(CC) $(CFLAGS) -c xor_decrypt.c

# Rule to clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Rule to run the program
run_encrypt: $(TARGET)
	./$(TARGET) encrypt inputfile.txt password123

run_decrypt: $(TARGET)
	./$(TARGET) decrypt new-inputfile.txt password123

.PHONY: all clean run_encrypt run_decrypt
