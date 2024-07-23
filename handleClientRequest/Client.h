#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <string>
#include "PeerConnection.h"

class Client {
private:
    std::string id;
    std::shared_ptr<PeerConnection> peerConnection_constructor;
public:
    Client(const std::string& clientId);
    std::shared_ptr<PeerConnection> getPeerConnection() const;
    void display() const;
};

#endif // CLIENT_H
