#pragma once

#include "../IComponent.hpp"
#include <SFML/System/Clock.hpp>
#include <vector>

class Strategy : public IComponent
{
private:
    std::vector<std::pair<std::vector<std::pair<std::pair<float, float>, float>>, int>> _patternList = {};
    std::vector<std::pair<std::pair<float, float>, float>> _pattern;
    int _patternHp = 0;
    sf::Clock clock;
    size_t arrayPos;
    void switchPattern(int);
public:
    Strategy(std::vector<std::pair<std::pair<float, float>, float>> strategy);
    Strategy(int number_of_element, int max_element_value, bool is_infinitly_looping);
    ~Strategy() override = default;

    // GET / SET
    std::vector<std::pair<std::pair<float, float>, float>> GetPatternFromHp(int);
    void SetPattern(std::vector<std::pair<std::pair<float, float>, float>>, int);

    // MODIFY PATTERN
    void AddElementToPattern(std::pair<std::pair<float, float>, float> element, int);
    void AddElementToPattern(std::pair<float, float>, int, int);

    void CreateRandomPattern(int, int, int, bool);
    std::pair<float, float> getDir(int);
    void RemoveElementToPattern(size_t index, int);
};
