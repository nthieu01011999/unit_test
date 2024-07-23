# Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -pthread

# Target executable name
TARGET = main

# Source files
SRCS = main.c message.c

# Header files
HEADERS = message.h

# Object directory
OBJDIR = obj

# Object files
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

# Default rule
all: $(TARGET)

# Rule to link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
$(OBJDIR)/%.o: %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule to clean up the build files
clean:
	rm -f $(TARGET) $(OBJDIR)/*.o
	rmdir $(OBJDIR) || true

# Rule to run the program
run: $(TARGET)
	./$(TARGET)
