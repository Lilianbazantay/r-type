#include <cstddef>
#include <iostream>
#include "Asio.hpp"

int main(int argc, char **argv) {
    Asio_network test(8080 + argc, nullptr);
    std::cout << argc << std::endl;
    test.start();
    if (argc == 1) {
        std::cout << "listening on port " << 8080 + argc << std::endl;
        while (!test.gotText) {}
        test.stop();
        return 0;
    }
    std::cout << "sending message" << std::endl;
    test.send(argv[1], "127.0.0.1", 8081);
    test.stop();
    return 83;
}