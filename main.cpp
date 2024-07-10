#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <vector>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/wait.h>
#include <vector>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <fstream>
#include <memory> 
#include "json.hpp" // Include for nlohmann::json

using json = nlohmann::json; // Alias nlohmann::json to json

typedef struct {
    std::string wSocketServerCfg; // store urls signaling
    std::vector<std::string> arrStunServerUrl;
    std::vector<std::string> arrTurnServerUrl;

    void clear() {
        wSocketServerCfg.clear();
        arrStunServerUrl.clear();
        arrTurnServerUrl.clear();
    }
} rtcServersConfig_t;

bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg);

void printRtcServersConfig(const rtcServersConfig_t& config) {
    std::cout << "STUN Server URLs:" << std::endl;
    for (const auto &Url : config.arrStunServerUrl) {
        std::cout << Url << std::endl;
    }

    std::cout << "TURN Server URLs:" << std::endl;
    for (const auto &Url : config.arrTurnServerUrl) {
        std::cout << Url << std::endl;
    }

    std::cout << "WebSocket URL:" << std::endl;
    std::cout << config.wSocketServerCfg << std::endl;
}

bool read_raw_file(std::string &data, const std::string &filename) {
    struct stat file_info;
    int file_obj = -1;
    char *buffer;

    file_obj = open(filename.data(), O_RDONLY);
    if (file_obj < 0) {
        return false;
    }
    fstat(file_obj, &file_info);
    buffer = (char*)malloc(file_info.st_size);
    if (buffer == NULL) {
        close(file_obj);
        return false;
    }
    ssize_t n = pread(file_obj, buffer, file_info.st_size, 0);
    (void)n;
    close(file_obj);

    data.assign((const char *)buffer, file_info.st_size);
    free(buffer);
    return true;
}

bool read_json_file(json &data, const std::string &filename) {
    std::string content;
    if (read_raw_file(content, filename)) {
        try {
            data = json::parse(content);
            return true;
        } catch (const std::exception &e) {
            (void)e;
        }
    }
    return false;
}

int configGetRtcServers(json &cfgJs, rtcServersConfig_t *rtcServerCfg) {
    if (mtce_jsonGetRtcServers(cfgJs, rtcServerCfg)) {
        return 0;
    } else {
        return -3;
    }
}

bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg) {
    if (rtcSvCfg == nullptr) {
        //APP_DBG("rtcSvCfg is null");
        return false;
    }

    try {
        rtcSvCfg->clear();
        json arrJs = json_in.at("Connections");
        
        for (const auto &server : arrJs) {
            if (server.contains("Type") && server["Type"].is_string()) {
                std::string type = server["Type"].get<std::string>();
                
                if (type == "stun") {
                    // Check if "StunUrl" is a std::string or an array and handle accordingly
                    if (server.contains("StunUrl")) {
                        if (server["StunUrl"].is_string()) {
                            rtcSvCfg->arrStunServerUrl.push_back(server["StunUrl"].get<std::string>());
                        } else if (server["StunUrl"].is_array() && !server["StunUrl"].empty()) {
                            rtcSvCfg->arrStunServerUrl.push_back(server["StunUrl"][0].get<std::string>());
                        }
                    }
                } else if (type == "turn") {
                    // Check if "TurnUrl" is a std::string or an array and handle accordingly
                    if (server.contains("TurnUrl")) {
                        if (server["TurnUrl"].is_string()) {
                            rtcSvCfg->arrTurnServerUrl.push_back(server["TurnUrl"].get<std::string>());
                        } else if (server["TurnUrl"].is_array() && !server["TurnUrl"].empty()) {
                            rtcSvCfg->arrTurnServerUrl.push_back(server["TurnUrl"][0].get<std::string>());
                        }
                    }
                } else if (type == "signaling") {
                    // Extract the signaling URL
                    if (server.contains("SignalingUrl") && server["SignalingUrl"].is_string()) {
                        rtcSvCfg->wSocketServerCfg = server["SignalingUrl"].get<std::string>();
                    } else {
                        //APP_DBG("Warning: 'SignalingUrl' field is missing or not a std::string");
                    }
                }
            } else {
                //APP_DBG("Warning: 'Type' field is missing or not a std::string");
            }
        }
        return true;
    }
    catch (const std::exception &e) {
        //APP_DBG("json error: %s", e.what());
        return false;
    }
}

int main() {
    std::string filename = "/home/it/Desktop/unit_test/Configuration/RtcServersConfig";
    std::string fileContent;
    json cfgJs;

    rtcServersConfig_t rtcServerCfg;

    if (!read_json_file(cfgJs, filename)) {
        std::cerr << "Failed to read the file: " << filename << std::endl;
    } else {
        int ret = configGetRtcServers(cfgJs, &rtcServerCfg);
        if (ret == 0) {
            printRtcServersConfig(rtcServerCfg);
        } else {
            std::cerr << "Failed to get RTC servers configuration." << std::endl;
        }
    }

    return 0;
}
