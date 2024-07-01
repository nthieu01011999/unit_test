#ifndef TIMER_UTILS_H
#define TIMER_UTILS_H

#include <iostream>
#include <string>

// Function to simulate client removal from a system
void removeClient(const std::string& clientId);

// Function to schedule client removal
void scheduleClientRemoval(const std::string& clientId);

#endif // TIMER_UTILS_H
