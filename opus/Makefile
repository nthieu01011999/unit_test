# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2 -I/usr/local/include/opus # Add -I/usr/local/include to include Opus headers

# Libraries
LIBS = -lasound -lopus

# Target executable name
TARGET = sound_cap

# Source files
SRCS = sound_cap.c

# Object files directory
OBJS_DIR = obj

# Object files (store in Obj folder)
OBJS = $(OBJS_DIR)/$(SRCS:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the Obj directory if it does not exist
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Rule to clean up generated files
clean:
	rm -f $(OBJS_DIR)/*.o $(TARGET)

# PHONY targets (not actual files)
.PHONY: all clean
