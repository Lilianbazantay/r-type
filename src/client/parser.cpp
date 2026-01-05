#include "parser.hpp"
#include "../utils.hpp"
#include <iostream>
#include <string>

/**
 * @brief Display the help message.
 */
void Parser::show_help()
{
    std::cout << "USAGE:\n"
              << "\t./client -h -i ip_address -p port\n"
              << "OPTIONS:\n"
              << "\t-h\t\tprint the help\n"
              << "\t-i --ip\t\tserver ip address\n"
              << "\t-p --port\tserver port (1024-65535)\n";
}

/**
 * @brief Parse the IP argument.
 *
 * @param arg Current argument being processed.
 * @param argv Argument vector.
 * @param i Current index in argv.
 * @param argc number of arguments.
 * @return true if parsing succeeded.
 * @return false if the IP argument is missing or invalid.
 */
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

/**
 * @brief Parse the port argument.
 *
 * @param arg Current argument being processed.
 * @param argv Argument vector.
 * @param i Current index in argv.
 * @param argc number of arguments.
 * @return true if parsing succeeded.
 * @return false if the port is missing, invalid, or out of range.
 */
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
/**
 * @brief Parse command-line arguments.
 *
 * @param argc number of arguments.
 * @param argv Argument vector.
 * @return int EXIT_SUCCESS if parsing succeeded, EXIT_FAILURE otherwise.
 */
int Parser::parse(int argc, char **argv)
{
    if (argc == 2 && std::string(argv[1]) == "-h") {
        show_help();
        return EXIT_ERROR;
    }
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (!parse_ip(arg, argv, i, argc)) return EXIT_ERROR;
        if (!parse_port(arg, argv, i, argc)) return EXIT_ERROR;
    }
    if (ip.empty() || port == -1) {
        std::cerr << "Missing required arguments.\n";
        show_help();
        return EXIT_ERROR;
    }
    return true;
}
