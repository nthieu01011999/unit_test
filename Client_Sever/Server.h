#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "Client.h"

class Server : public std::enable_shared_from_this<Server> {
public:
    // Add client to the server
    void addClient(std::shared_ptr<Client> client);

    // Remove client from the server
    void removeClient(int id);

    // List all connected clients
    void listClients();

private:
    std::vector<std::shared_ptr<Client>> clients;
};

#endif // SERVER_H
