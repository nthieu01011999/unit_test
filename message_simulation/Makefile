#Step 1: Define Basic Variables

CXX = g++
CXXFLAGS = -Wall -Wextra -g
TARGET = main

all: $(TARGET)

# g++ -o message_simulation message_simulation.cpp
# ./message_simulation

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
