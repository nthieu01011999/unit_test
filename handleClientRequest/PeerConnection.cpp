#include "PeerConnection.h"

PeerConnection::PeerConnection() {
    std::cout << "PeerConnection Created\n";
}

void PeerConnection::connect() {
    std::cout << "PeerConnection is now connected.\n";
}

void PeerConnection::enableFeature(const std::string& feature) {
    std::cout << "Feature " << feature << " enabled on PeerConnection.\n";
}
