#ifndef PEER_CONNECTION_H
#define PEER_CONNECTION_H

#include <iostream>

class PeerConnection {
public:
    PeerConnection();
    void connect();
    void enableFeature(const std::string& feature);
};

#endif // PEER_CONNECTION_H
