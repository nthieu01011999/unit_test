#include "timer_utils.h"
#include "cpptime.h"
#include <iostream>
#include <string>

CppTime::Timer systemTimer;

void removeClient(const std::string& clientId) {
    std::cout << "Removing client with ID: " << clientId << std::endl;
}

void scheduleClientRemoval(const std::string& clientId) {
    systemTimer.add(std::chrono::milliseconds(100), [clientId](CppTime::timer_id) {
        removeClient(clientId);
    });
}
