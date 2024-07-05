#include "Client.h"
#include "Server.h"
#include <ctime>

Client::Client(int id) : id(id) {
    std::cout << "Client " << id << " created.\n";
    logFile.open("client_" + std::to_string(id) + "_log.txt", std::ios_base::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file for client " << id << ".\n";
    }
}

Client::~Client() {
    std::cout << "Client " << id << " destroyed.\n";
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Client::connectToServer(std::shared_ptr<Server> server) {
    this->weak_server = server;
    std::cout << "Client " << id << " connected to server.\n";
    logInteraction("Connected to server");
}

void Client::interactWithServer() {
    if (auto spt = weak_server.lock()) { // Check if server still exists
        std::cout << "Client " << id << " interacting with server.\n";
        logInteraction("Interacted with server");
    } else {
        std::cout << "Client " << id << " failed to interact: server not available.\n";
        logInteraction("Failed to interact: server not available");
    }
}

int Client::getId() const {
    return id;
}

void Client::logInteraction(const std::string &message) {
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        logFile << std::ctime(&now) << " - Client " << id << ": " << message << '\n';
    }
}
