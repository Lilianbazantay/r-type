#include "ConfigParser.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

/**
 * @brief Loads a JSON configuration file for prototypes.
 *
 * @param path Path to the JSON file
 * @return true if the file was successfully loaded and parsed
 * @return false if an error occurred (cannot open file, parse error, or invalid content)
 */
bool ConfigParser::loadFile(const std::string &path)
{
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

    if (!root.is_object() || !root.contains("entity")) {
        std::cerr << "ConfigParser::loadFile - invalid entity file\n";
        return false;
    }

    const json& entity = root["entity"];

    if (!entity.contains("name")) {
        std::cerr << "ConfigParser::loadFile - entity has no name\n";
        return false;
    }

    Prototype proto;
    proto.name = entity["name"].get<std::string>();
    proto.data = root;

    prototypes.emplace(proto.name, std::move(proto));
    return true;
}
