#include <iostream>
#include <unordered_map>
#include "Client.h"

int main() {
    std::unordered_map<std::string, std::shared_ptr<Client>> clients;
    std::unordered_map<std::string, std::shared_ptr<PeerConnection>> peerConnections;

    // Creating a new client
    std::string clientId = "client123";
    clients[clientId] = std::make_shared<Client>(clientId);

    // Storing the associated PeerConnection
    peerConnections[clientId] = clients[clientId]->getPeerConnection();

    // Displaying client info and connecting
    clients[clientId]->display();
    peerConnections[clientId]->connect();

    return 0;
}
