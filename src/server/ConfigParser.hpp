#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Prototype.hpp"

class ConfigParser {
public:
    ConfigParser() = default;
    ~ConfigParser() = default;

    bool loadFile(const std::string &path);

    const std::unordered_map<std::string, Prototype>& getPrototypes() const {
        return prototypes;
    }

private:
    std::unordered_map<std::string, Prototype> prototypes;
};
