#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>
#include "websocket.hpp"

// Global shared pointer to WebSocket
std::shared_ptr<WebSocket> globalWebSocket;

// Global weak pointer to WebSocket
std::weak_ptr<WebSocket> globalWeakWebSocket;

void initializeWebSocketServer(const std::string &wsUrl);
void periodicWebSocketCheck();

int main() {
    // Create a shared pointer to a WebSocket object
    auto ws = std::make_shared<WebSocket>();
    
    // Create a weak pointer to the WebSocket object
    std::weak_ptr<WebSocket> wws = ws;

    // Assign to global shared pointer
    globalWebSocket = ws;

    // Assign to global weak pointer
    globalWeakWebSocket = wws;

    // Initialize WebSocket server
    initializeWebSocketServer("ws://sig.espitek.com:8089/c02i24010000008");

    // Start a thread to periodically check the WebSocket status
    std::thread checker(periodicWebSocketCheck);
    checker.detach();

    // Simulate WebSocket events after a delay to see the output clearly
    std::this_thread::sleep_for(std::chrono::seconds(3));
    globalWebSocket->send("Test message from client");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    globalWebSocket->close();

    // Wait for a bit before exiting to see periodic checks
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}

void initializeWebSocketServer(const std::string &wsUrl) {
    // Event handler for connection open
    globalWebSocket->onOpen([&]() {
        std::cout << "WebSocket connection established.\n";
    });

    // Event handler for connection close
    globalWebSocket->onClose([&]() {
        std::cout << "WebSocket connection closed.\n";
    });

    // Event handler for errors
    globalWebSocket->onError([](const std::string &msg) {
        std::cout << "WebSocket error: " << msg << "\n";
    });

    // Event handler for incoming messages
    globalWebSocket->onMessage([](std::variant<std::vector<uint8_t>, std::string> data) {
        if (std::holds_alternative<std::string>(data)) {
            std::string message = std::get<std::string>(data);
            std::cout << "WebSocket message received: " << message << "\n";
        } else {
            std::cout << "WebSocket binary message received.\n";
        }
    });

    // Open WebSocket connection
    globalWebSocket->open(wsUrl);
}

void periodicWebSocketCheck() {
    while (true) {
        auto wwwwws = globalWeakWebSocket.lock();
        if (wwwwws) {
            // WebSocket still exists and connection might be active
            std::cout << "WebSocket is still active.\n";
        } else {
            // WebSocket has been destroyed
            std::cout << "WebSocket no longer exists.\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
