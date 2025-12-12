#pragma once

#include <string>
#include <nlohmann/json.hpp>

struct Prototype {
    std::string name;
    nlohmann::json data;
};
