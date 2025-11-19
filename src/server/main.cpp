#include "../utils.hpp"
#include "parser.hpp"

int main(int argc, char **argv) {
    Parser parser;

    if (parser.ParseData(argc, argv) == EXIT_ERROR)
        return EXIT_ERROR;
    return EXIT_ERROR;
}
