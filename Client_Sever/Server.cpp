#include "Server.h"

void Server::addClient(std::shared_ptr<Client> client) {
    clients.push_back(client);
    client->connectToServer(shared_from_this());
}

void Server::listClients() {
    std::cout << "Server has " << clients.size() << " clients connected.\n";
}