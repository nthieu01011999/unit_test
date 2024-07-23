#include "Client.h"
#include <iostream>

Client::Client(const std::string& clientId) : id(clientId) {
    peerConnection_constructor = std::make_shared<PeerConnection>();
}

std::shared_ptr<PeerConnection> Client::getPeerConnection() const {
    return peerConnection_constructor;
}

void Client::display() const {
    std::cout << "Client ID: " << id << " with PeerConnection\n";
}
