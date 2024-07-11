CC = g++
CFLAGS = -std=c++11 -Wall
OBJDIR = Obj

all: main

main: $(OBJDIR)/main.o $(OBJDIR)/Client.o $(OBJDIR)/PeerConnection.o
	$(CC) $(CFLAGS) -o main $^

$(OBJDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o $@

$(OBJDIR)/Client.o: Client.cpp
	$(CC) $(CFLAGS) -c Client.cpp -o $@

$(OBJDIR)/PeerConnection.o: PeerConnection.cpp
	$(CC) $(CFLAGS) -c PeerConnection.cpp -o $@

clean:
	rm -f $(OBJDIR)/*.o main

# Ensure the Obj directory exists
$(shell mkdir -p $(OBJDIR))
