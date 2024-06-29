// Configuration.hpp
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>

struct Configuration {
    std::string iceServerUrl;
    bool useRelay;
    int maxBandwidth;

    Configuration(const std::string& url, bool relay, int bandwidth)
        : iceServerUrl(url), useRelay(relay), maxBandwidth(bandwidth) {}
};

#endif // CONFIGURATION_HPP
