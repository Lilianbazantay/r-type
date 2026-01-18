#include <iostream>
#include <exception>

#include "GameEditor.hpp"

/**
 * @brief Launch the application of configuration into a try catch
 *
 * @return int return 0 when it exit successfuly and 84 if it fail
 */
int main()
{
    try {
        GameEditor editor;
        editor.runGameEditor();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 84;
    }
    catch (...) {
        std::cerr << "Unknown fatal error." << std::endl;
        return 84;
    }
    return 0;
}
