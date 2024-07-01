CC = g++
CFLAGS = -std=c++11 -Wall

all: main

main: main.o Client.o PeerConnection.o
	$(CC) $(CFLAGS) -o main main.o Client.o PeerConnection.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Client.o: Client.cpp
	$(CC) $(CFLAGS) -c Client.cpp

PeerConnection.o: PeerConnection.cpp
	$(CC) $(CFLAGS) -c PeerConnection.cpp

clean:
	rm -f *.o main
