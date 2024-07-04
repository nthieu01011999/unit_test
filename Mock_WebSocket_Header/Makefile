CXX = g++
CXXFLAGS = -Wall -std=c++17 -I/home/it/Desktop/unit_test/Mock_WebSocket_Header

all: main

main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f main main.o
