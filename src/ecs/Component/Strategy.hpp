#pragma once

#include "../IComponent.hpp"
#include <vector>

/**
 * @brief All action possible for the strategy
 */

#define NUMBER_STRATEGY_POSSIBILITY 6

enum StrategyType {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    SHOOT,
    LOOP,
};

class Strategy : public IComponent
{
private:
    std::vector<std::pair<StrategyType, int>> _pattern;

public:
    Strategy(std::vector<std::pair<StrategyType, int>> strategy);
    ~Strategy() override = default;

    // GET / SET
    std::vector<std::pair<StrategyType, int>> GetPattern();
    void SetPattern(std::vector<std::pair<StrategyType, int>> new_pattern);

    // MODIFY PATTERN
    void AddElementToPattern(std::pair<StrategyType, int> element);
    void AddElementToPattern(StrategyType action, int value);

    std::vector<std::pair<StrategyType, int>> CreateRandomPattern(int number_of_element, int max_element_value, bool is_infinitly_looping);

    void RemoveElementToPattern(size_t index);
};
