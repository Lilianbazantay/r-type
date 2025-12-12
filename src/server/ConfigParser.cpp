#include "ConfigParser.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool ConfigParser::loadFile(const std::string &path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        std::cerr << "ConfigParser::loadFile - cannot open file: " << path << "\n";
        return false;
    }

    json root;
    try {
        ifs >> root;
    } catch (const json::parse_error &e) {
        std::cerr << "ConfigParser::loadFile - parse error: " << e.what() << "\n";
        return false;
    }

    if (!root.is_object()) {
        std::cerr << "ConfigParser::loadFile - root is not an object\n";
        return false;
    }

    for (auto it = root.begin(); it != root.end(); ++it) {
        Prototype p;
        p.name = it.key();
        p.data = it.value();
        prototypes.emplace(p.name, std::move(p));
    }

    return true;
}
