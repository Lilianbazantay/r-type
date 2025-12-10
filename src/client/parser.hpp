#ifndef CLIENT_PARSER
    #define CLIENT_PARSER


#pragma once

#include <string>

/**
 * @brief parse all argument given by the user and print usage if argument are invalid
 */
class Parser {
private:
    void show_help();
    bool parse_ip(const std::string &arg, char **argv, int &i, int argc);
    bool parse_port(const std::string &arg, char **argv, int &i, int argc);
public:
    std::string ip;
    int port = -1;
    int parse(int argc, char **argv);
};

#endif
