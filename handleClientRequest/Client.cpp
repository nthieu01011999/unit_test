#include "Client.h"
#include <iostream>

Client::Client(const std::string& clientId) : id(clientId) {
    peerConnection = std::make_shared<PeerConnection>();
}

std::shared_ptr<PeerConnection> Client::getPeerConnection() const {
    return peerConnection;
}

void Client::display() const {
    std::cout << "Client ID: " << id << " with PeerConnection\n";
}
