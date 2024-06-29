// main.cpp
#include <memory>
#include "PeerConnection.hpp"

int main() {
    //1. Constructor of Configuration
    Configuration config("stun.example.com", true, 100);
    auto pc = std::make_shared<PeerConnection>(config);

    return 0;
}