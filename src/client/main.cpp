#include "./graphical/Game.hpp"
#include "parser.hpp"
#include "client.hpp"
#include <thread>
#include <iostream>
#include <sstream>

void clientConsoleThread()
{
    Parser parser;

    const char* argv_fake[] = {"prog", "127.0.0.1", "4242"};
    parser.parse(3, const_cast<char**>(argv_fake));

    Client client(parser.ip, parser.port);
    client.start();

    std::cout << "Connected to " << parser.ip << ":" << parser.port << "\n";

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "quit" || input == "exit") {
            break;
        }

        if (input == "start") {
            client.sendStartGame();
            continue;
        }

        if (input.rfind("press ", 0) == 0) {
            std::string key = input.substr(6);
            uint8_t code;
            if (key == "fire") code = 0;
            else if (key == "up") code = 1;
            else if (key == "down") code = 2;
            else if (key == "left") code = 3;
            else if (key == "right") code = 4;
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
            if (key == "fire") code = 0;
            else if (key == "up") code = 1;
            else if (key == "down") code = 2;
            else if (key == "left") code = 3;
            else if (key == "right") code = 4;
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
}

int main()
{
    std::thread consoleThread(clientConsoleThread);

    try {
        Game app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    if (consoleThread.joinable())
        consoleThread.join();

    return 0;
}
