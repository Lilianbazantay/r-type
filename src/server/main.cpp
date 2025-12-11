#include "utils.hpp"
#include "parser.hpp"
#include "server.hpp"

#include <iostream>


/**
 * @brief Main function that launch the programe
 *
 * @param argc Number of argument
 * @param argv List of argument
 * @return int Return into the terminal EXIT_ERROR(= 84) if the programe fail
 * and EXIT_SUCCESS if the programe didn't fail
 */
int main(int argc, char **argv) {
    Parser parser;

    if (parser.ParseData(argc, argv) == EXIT_ERROR)
        return EXIT_ERROR;
    Server test(parser.getPort());
    test.start();
    if (argc >= 2) {
        std::cout << "listening on port " << parser.getPort() << std::endl;
        while (!test.gotText)
        test.stop();
        return EXIT_DESIRED;
    }
    std::cout << "sending message" << std::endl;
    test.stop();
    return EXIT_DESIRED;
}
