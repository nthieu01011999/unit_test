#include "config_structures.h"
#include "utility_functions.h"
#include "error_handler.h"
#include <iostream>

void printRtcServersConfig(const rtcServersConfig_t& config) {
    std::cout << "STUN Server URLs:" << std::endl;
    for (const auto &Url : config.arrStunServerUrl) {
        std::cout << Url << std::endl;
    }

    std::cout << "TURN Server URLs:" << std::endl;
    for (const auto &Url : config.arrTurnServerUrl) {
        std::cout << Url << std::endl;
    }

    // std::cout << "Websocket URLs:" << std::endl;
    // for (const auto &Url : config.wSocketServerCfg) {
    //     std::cout << Url << std::endl;
    // }
}

int main() {
    Configuration rtcConfig;
    rtcServersConfig_t rtcServerCfg;
    ErrorHandler errorHandler;

    try {
        configGetRtcServers(&rtcServerCfg);
        printRtcServersConfig(rtcServerCfg);
        // json cfgJs;
        // std::string fileName = "RtcServersConfig";
        // mtce_readConfigUsrDfaulFileToJs(cfgJs, fileName);
    } catch (const std::exception& e) {
        errorHandler.handleError(e.what());
    }

    return 0;
}