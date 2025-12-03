#include "parser.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

void Parser::show_help()
{
    std::cout << "USAGE:\n"
              << "\t./client -h -i ip_address -p port\n"
              << "OPTIONS:\n"
              << "\t-h\t\tprint the help\n"
              << "\t-i --ip\t\tserver ip address\n"
              << "\t-p --port\tserver port (1024-65535)\n";
}

bool Parser::parse_ip(const std::string &arg, char **argv, int &i, int argc)
{
    if (arg == "-i" || arg == "--ip") {
        if (i + 1 < argc) {
            ip = argv[i + 1];
            i++;
            return true;
        }
        std::cerr << "Missing IP after " << arg << "\n";
        return false;
    }
    return true;
}

bool Parser::parse_port(const std::string &arg, char **argv, int &i, int argc)
{
    if (arg == "-p" || arg == "--port") {
        if (i + 1 < argc) {
            try {
                int p = std::stoi(argv[i + 1]);
                if (p < 1024 || p > 65535) {
                    std::cerr << "Invalid port: " << p << "\n";
                    return false;
                }
                port = p;
                i++;
                return true;
            } catch (...) {
                std::cerr << "Invalid port format\n";
                return false;
            }
        }
        std::cerr << "Missing port after " << arg << "\n";
        return false;
    }
    return true;
}

int Parser::parse(int argc, char **argv)
{
    if (argc == 2 && std::string(argv[1]) == "-h") {
        show_help();
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (!parse_ip(arg, argv, i, argc)) return EXIT_FAILURE;
        if (!parse_port(arg, argv, i, argc)) return EXIT_FAILURE;
    }
    if (ip.empty() || port == -1) {
        std::cerr << "Missing required arguments.\n";
        show_help();
        return EXIT_FAILURE;
    }
    return 0;
}
