# Define compiler
CC=g++

# Compiler flags
CFLAGS=-Wall -std=c++11

# Linker flags
LDFLAGS=-lpthread

# Define the executable output
EXEC=clientManager

# Source files
SOURCES=main.cpp timer_utils.cpp

# Object files
OBJECTS=$(SOURCES:.cpp=.o)

# Default target
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f $(OBJECTS) $(EXEC)

# Phony targets
.PHONY: clean
