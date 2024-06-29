#ifndef PEER_CONNECTION_HPP
#define PEER_CONNECTION_HPP

#include "Configuration.hpp"
#include <iostream>

class PeerConnection {
public:
    PeerConnection(const Configuration& config) {
        std::cout << "Initializing PeerConnection with the following settings:\n"
                  << "ICE Server URL: " << config.iceServerUrl << "\n"
                  << "Use Relay: " << (config.useRelay ? "Yes" : "No") << "\n"
                  << "Max Bandwidth: " << config.maxBandwidth << " Mbps\n";
    }

    ~PeerConnection() {
        std::cout << "PeerConnection destroyed.\n";
    }
};

#endif //PEER_CONNECTION_HPP