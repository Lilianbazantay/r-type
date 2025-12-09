#ifndef UTILS
    #define UTILS

#include <cstddef>
#include <format>
#include <string>
#include <vector>
#pragma once

constexpr int EXIT_ERROR = 84;
constexpr int EXIT_DESIRED = 0;

std::string ipToString(std::vector<size_t> ip) {
    std::string stringIP = "";
    for (size_t i = 0; i < ip.size(); i++)
        stringIP += std::format("{}{}", ip[i], (i + 1 < ip.size() ? "." : ""));
    return stringIP;
};

#endif
