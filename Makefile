# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2 -I/usr/local/include/opus # Add -I/usr/local/include to include Opus headers

# Libraries
LIBS = -lasound -lopus

# Target executable name
TARGET = sound_cap

# Source files
SRCS = sound_cap.c

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable directly from the source files
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Rule to clean up generated files
clean:
	rm -f $(TARGET)

# PHONY targets (not actual files)
.PHONY: all clean
