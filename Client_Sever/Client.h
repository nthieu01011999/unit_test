#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <memory>
#include <fstream>
#include <string>

// Forward declaration of Server class
class Server;

class Client {
public:
    // Constructor
    Client(int id);

    // Destructor
    ~Client();

    // Connect client to server
    void connectToServer(std::shared_ptr<Server> server);

    // Interact with the server
    void interactWithServer();

    // Get client ID
    int getId() const;

private:
    int id;
    std::weak_ptr<Server> weak_server; // Weak reference to avoid circular dependency
    std::ofstream logFile;

    void logInteraction(const std::string &message);
};

#endif // CLIENT_H
