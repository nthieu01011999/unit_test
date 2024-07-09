#include "error_handler.h"
#include <iostream>

void ErrorHandler::handleError(const std::string& error ) {
    std::cout << "Error: " << error << std::endl;
}