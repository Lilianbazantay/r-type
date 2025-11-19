#pragma once

#include <string>

/**
 * @brief parse all argument given by the user and print usage if argument are invalid
 */
class Parser
{
private:
    std::string ip;
    int port;
public:
    ~Parser() = default;

    static void descripton();
    int ParseData(int argc, char **argv);
};
