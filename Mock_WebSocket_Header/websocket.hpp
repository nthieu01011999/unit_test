#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>

// Simplified WebSocket class
class WebSocket {
public:
    using MessageHandler = std::function<void(std::variant<std::vector<uint8_t>, std::string>)>;
    using ErrorHandler = std::function<void(const std::string&)>;
    using ConnectionHandler = std::function<void()>;

    WebSocket() = default;
    ~WebSocket() = default;

    void open(const std::string& url) {
        std::cout << "Opening WebSocket connection to " << url << std::endl;
        if (onOpen_) onOpen_();
    }

    void close() {
        std::cout << "Closing WebSocket connection." << std::endl;
        if (onClose_) onClose_();
    }

    void send(const std::string& message) {
        std::cout << "Sending message: " << message << std::endl;
        if (onMessage_) onMessage_(message);
    }

    void onOpen(ConnectionHandler handler) {
        onOpen_ = handler;
    }

    void onClose(ConnectionHandler handler) {
        onClose_ = handler;
    }

    void onError(ErrorHandler handler) {
        onError_ = handler;
    }

    void onMessage(MessageHandler handler) {
        onMessage_ = handler;
    }

private:
    ConnectionHandler onOpen_;
    ConnectionHandler onClose_;
    ErrorHandler onError_;
    MessageHandler onMessage_;
};

#endif // WEBSOCKET_HPP
