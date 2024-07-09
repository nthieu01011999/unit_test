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
#include <exception>

#include "utility_functions.h"  // Include your header for the function declaration


int configGetRtcServers(rtcServersConfig_t *rtcServerCfg) {
	json cfgJs;

	std::string fileName = "RtcServersConfig";
	if (mtce_readConfigUsrDfaulFileToJs(cfgJs, fileName)) {
		return -1;
	}

	if (mtce_jsonGetRtcServers(cfgJs, rtcServerCfg)) {
		std::cout << "rtc server config:\n" << cfgJs.dump().data() << std:: endl;
		return 0;
	}
	else {
		
        std::cout << "rtc server config: %s\n" << fileName.data() << std:: endl;
		return -3;
	}
}

int mtce_readConfigUsrDfaulFileToJs(json &cfgJs, std::string &file) {
	std::string filePath = "/home/it/Desktop/unit_test/Configuration" + std::string("/") + file;
	if (!read_json_file(cfgJs, filePath)) {
		std::cout << "Can not read: %s\n" <<  filePath.data() << std::endl;
		filePath = "/app/default" + std::string("/") + file;
		cfgJs.clear();
		if (!read_json_file(cfgJs, filePath)) {
			// APP_DBG("Can not read: %s\n", filePath.data());
            std::cout << "Can not read: %s\n" <<  filePath.data() << std::endl;
			return -1;
		}
	}
	return 0;
}

bool read_json_file(json &data, const std::string &filename) {
    std::string content;
    if (read_raw_file(content, filename)) {
        try {
            data = json::parse(content);
            return true;
        } catch (const std::exception &e) {
            (void)e;
            std::cerr << "json::parse()\n" << e.what() << std::endl;
        }
    }
    return false;
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
	buffer = (char *)malloc(file_info.st_size);
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


bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg) {
    if (rtcSvCfg == nullptr) {
        // APP_DBG("rtcSvCfg is null");
        return false;
    }

    try {
        rtcSvCfg->clear();
        json arrJs = json_in.at("Connections");
        
        for (const auto &server : arrJs) {
            if (server.contains("Type") && server["Type"].is_string()) {
                std::string type = server["Type"].get<std::string>();
                
                if (type == "stun") {
                    // Check if "StunUrl" is a string or an array and handle accordingly
                    if (server.contains("StunUrl")) {
                        if (server["StunUrl"].is_string()) {
                            rtcSvCfg->arrStunServerUrl.push_back(server["StunUrl"].get<std::string>());
                        } else if (server["StunUrl"].is_array() && !server["StunUrl"].empty()) {
                            rtcSvCfg->arrStunServerUrl.push_back(server["StunUrl"][0].get<std::string>());
                        }
                    }
                } else if (type == "turn") {
                    // Check if "TurnUrl" is a string or an array and handle accordingly
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
                        // APP_DBG("Warning: 'SignalingUrl' field is missing or not a string");
                    }
                }
            } else {
                // APP_DBG("Warning: 'Type' field is missing or not a string");
            }
        }
        return true;
    }
    catch (const std::exception &e) {
        // APP_DBG("json error: %s", e.what());
        std::cerr << "json error: %s" << e.what() << std::endl;
        return false;
    }
}