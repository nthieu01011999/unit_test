#ifndef CONFIG_STRUCTURES_H
#define CONFIG_STRUCTURES_H

#include <string>
#include <vector>

struct IceServer {
    std::string hostname;
    int port;
    std::string username;
    std::string password;
};

struct Configuration {
    std::vector<IceServer> iceServers;
};

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

#endif // CONFIG_STRUCTURES_H
