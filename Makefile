# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2 -I/usr/local/include/opus # Add -I/usr/local/include to include Opus headers

# Libraries
LIBS = -lasound -lopus

# Target executable name
TARGET = sound_cap

# Source files
SRCS = sound_cap.c

# Object files (derived from the source files)
OBJS = $(SRCS:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# PHONY targets (not actual files)
.PHONY: all clean
