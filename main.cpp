#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void parseAndRouteMessage(const std::string& jsonString); 

void handleRequest(const nlohmann::json& message) {
    std::cout << "Handling request message" << std::endl;
    // Add your request handling logic here
}

int main() {
    std::string jsonString = R"({
                                "ClientId": "lHv1w7W3uZ",
                                "Type": "request"
                            })";

    parseAndRouteMessage(jsonString);
    return 0;
}

void parseAndRouteMessage(const std::string& jsonString) {
    try {
        nlohmann::json message = nlohmann::json::parse(jsonString);
        std::string type = message.at("Type").get<std::string>();

        if (type == "request") {
            handleRequest(message);
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}