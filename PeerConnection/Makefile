# Compiler and linker configurations
CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS =

# Define the names of the output file
TARGET = peer_connection_app

# Specify object files and source files
OBJ = main.o
SRC = main.cpp

# Default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compilation rules
main.o: main.cpp Configuration.hpp PeerConnection.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
