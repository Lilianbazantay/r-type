#include "../utils.hpp"
#include "parser.hpp"
#include "client.hpp"
#include <iostream>
#include <thread>
#include <sstream>

/**
 * @brief Main function that launches the program
 *
 * @param argc number of arguments.
 * @param argv Argument vector.
 * @return 0 if parsing succeeded, 84 otherwise.
 */
int main(int argc, char **argv)
{
    Parser parser;

    if (parser.parse(argc, argv) != EXIT_SUCCESS)
        return 84;
    Client client(parser.ip, parser.port);
    client.start();
    std::cout << "Connected to " << parser.ip << ":" << parser.port << "\n";
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "quit" || input == "exit")
            break;
        if (input == "start") {
            client.sendStartGame();
            continue;
        }
        if (input.rfind("press ", 0) == 0) {
            std::string key = input.substr(6);
            uint8_t code;
            if      (key == "fire") code = 0;
            else if (key == "up")   code = 1;
            else if (key == "down") code = 2;
            else if (key == "left") code = 3;
            else if (key == "right")code = 4;
            else {
                std::cout << "Unknown key: " << key << "\n";
                continue;
            }
            client.sendInput(true, code);
            continue;
        }
        if (input.rfind("release ", 0) == 0) {
            std::string key = input.substr(8);
            uint8_t code;
            if      (key == "fire") code = 0;
            else if (key == "up")   code = 1;
            else if (key == "down") code = 2;
            else if (key == "left") code = 3;
            else if (key == "right")code = 4;
            else {
                std::cout << "Unknown key: " << key << "\n";
                continue;
            }
            client.sendInput(false, code);
            continue;
        }
        if (input.rfind("connect ", 0) == 0) {
            std::stringstream ss(input);
            std::string cmd, ipStr;
            int port;
            ss >> cmd >> ipStr >> port;
            if (ipStr.empty() || port <= 0) {
                std::cout << "Usage: connect <ip> <port>\n";
                continue;
            }
            uint32_t ipValue = 0;
            unsigned a, b, c, d;
            if (sscanf(ipStr.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
                ipValue = (a << 24) | (b << 16) | (c << 8) | d;
            } else {
                std::cout << "Invalid IP format\n";
                continue;
            }
            client.sendConnectionRequest(ipValue, port);
            continue;
        }
        std::cout << "Unknown command. Type 'help'.\n";
    }
    return 0;
}
