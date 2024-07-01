#include <iostream>
#include <thread>
#include <chrono>
#include "timer_utils.h"

int main() {
    std::string clientId = "client123";
    std::cout << "Client " << clientId << " has disconnected. Scheduling removal..." << std::endl;

    // Schedule the removal of the client after 100 milliseconds
    scheduleClientRemoval(clientId);

    // Simulate some other work to keep the application running and see the output
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
