#include <iostream>
#include <exception>

#include "GameEditor.hpp"

int main()
{
    try {
        GameEditor editor;
        editor.RunGameEditor();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown fatal error." << std::endl;
        return 1;
    }

    return 0;
}
