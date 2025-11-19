#include "parser.hpp"
#include "../utils.hpp"

#include <iostream>
#include <string>

/**
 * @brief Display the usage into the terminal
 */
void Parser::descripton()
{
    std::cout << "USAGE:\n"
              << "\t./client -h -i ip_address -p port\n"
              << "OPTIONS:\n"
              << "\t-h\t\tprint the help\n"
              << "\t-i or --ip\t\tspecify the ip address of the server\n"
              << "\t-p or --port\t\tspecify the port of the server\n";
}

/**
 * @brief Parse all data given as argument and store the into the 'Parser' class
 *
 * @param argc number of argument
 * @param argv list of argument
 * @return int 'EXIT_ERROR' if argument format are invalid else return 'EXIT_SUCCESS'
 */
int Parser::ParseData(int argc, char **argv)
{
    if (argc < 3) {
        descripton();
        return EXIT_ERROR;
    }
    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h")
            descripton();
        if (arg == "-i" || arg == "--ip") {
            if (i + 1 <= argc) {
                this->ip = argv[i + 1];
            } else {
                return EXIT_ERROR;
            }
        }
        if (arg == "-p" || arg == "--port") {
            if (i + 1 <= argc) {
                try {
                    int tmpPort = std::stoi(argv[i + 1]);
                    if (tmpPort < 1024 || tmpPort > 65535) {
                        std::cerr << "Invalid port value" << argv[i + 1]
                        << "\nUse \"./client -h\" to get help\n";
                        return EXIT_FAILURE;
                    } else {
                        this->port = tmpPort;
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Invalid port value: " << argv[i + 1]
                    << "\nUse \"./client -h\" to get help\n";
                    return EXIT_ERROR;
                }
            } else {
                return EXIT_ERROR;
            }
        }
    }
    return EXIT_SUCCESS;
}
