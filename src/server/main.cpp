#include "server/ServerGame.hpp"
#include "utils.hpp"
#include "parser.hpp"
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

    NetworkServerBuffer netReceiveBuffer;
    NetworkClientBuffer netSendBuffer;
    NetworkContinuousBuffer netContinuousBuffer;
    if (parser.ParseData(argc, argv) == EXIT_ERROR)
        return EXIT_ERROR;
    ServerGame test(parser.getPort(), &netReceiveBuffer, &netSendBuffer, &netContinuousBuffer);
    std::cerr << "[DEBUG] Starting ServerGame loop\n";
    test.Loop();
    std::cerr << "[DEBUG] ServerGame loop Finished\n";
    //if (argc >= 2) {
    //    std::cout << "listening on port " << parser.getPort() << std::endl;
    //    while (1);
    //    return EXIT_DESIRED;
    //}
    return EXIT_DESIRED;
}
