#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H
#include<string>
#include "json.hpp" // Include for nlohmann::json

#include "config_structures.h"


#include <nlohmann/json.hpp>
using json = nlohmann::json;

int configGetRtcServers(rtcServersConfig_t *rtcServerCfg);
int mtce_readConfigUsrDfaulFileToJs(json &cfgJs, std::string &file);
bool read_raw_file(std::string &data, const std::string &filename);
bool read_json_file(json &data, const std::string &filename);
bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg);
#endif // UTILITY_FUNCTIONS_H
