#include "../utils.hpp"
#include "parser.hpp"
#include "client.hpp"
#include <iostream>
#include <thread>

/**
 * @brief Main function that launch the programe
 *
 * @param argc Number of argument
 * @param argv List of argument
 * @return int Return into the terminal EXIT_ERROR(= 84) if the programe fail
 * and EXIT_SUCCESS if the programe didn't fail
 */
int main(int argc, char **argv)
{
    Parser parser;

    if (parser.parse(argc, argv) != EXIT_SUCCESS)
        return 84;
    Client client(parser.ip, parser.port);
    client.start();
    std::cout << "Connected to " << parser.ip << ":" << parser.port << "\n";
    std::string userInput;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);
        if (userInput == "quit" || userInput == "exit")
            break;
        client.send_message(userInput);
    }
    return 0;
}
