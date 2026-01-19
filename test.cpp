#include <iostream>
#include <vector>

int main(void) {
    std::vector<int> test0 = {0, 12, 43, 5};
    std::vector<std::vector<int>> test1 = {{}, {}, test0, {}};

    for (size_t i = 0; i < test1.size(); i++)
        if (test1[i] == test0)
            std::cout << "Same!" << std::endl;
}