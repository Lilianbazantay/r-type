#include "parser.hpp"
#include "../utils.hpp"

#include <iostream>
#include <string>

void Parser::descripton()
{
    std::cout << "USAGE:\n"
              << "\t./server -h -p port\n"
              << "OPTIONS:\n"
              << "\t-h\t\tprint the help\n"
              << "\t-p or --port\t\tspecify the port number between 1064 and 65535\n";
}

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
        if (arg == "-p" || arg == "--port") {
            if (i + 1 <= argc) {
                try {
                    int tmpPort = std::stoi(argv[i + 1]);
                    if (tmpPort < 1024 || tmpPort > 65535) {
                        std::cerr << "Invalid port value" << argv[i + 1]
                        << "\nUse \"./serveur -h\" to get help\n";
                        return EXIT_FAILURE;
                    } else {
                        this->port = tmpPort;
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Invalid port value: " << argv[i + 1]
                    << "\nUse \"./serveur -h\" to get help\n";
                    return EXIT_ERROR;
                }
            } else {
                return EXIT_ERROR;
            }
        }
    }
    return EXIT_SUCCESS;
}
