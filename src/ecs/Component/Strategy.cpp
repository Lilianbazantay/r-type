#include "Strategy.hpp"
#include "../IComponent.hpp"
#include <cstdlib>

/**
 * @brief Construct a new Strategy:: Strategy object
 *
 * @param strategy Set the strategy pattern
 */
Strategy::Strategy(std::vector<std::pair<StrategyType, int>> pattern): _pattern(pattern)
{
    _type = ComponentType::STRATEGY;
}

/**
 * @brief Return the pattern
 *
 * @return std::vector<std::pair<StrategyType, int>> Pattern type returned
 */
std::vector<std::pair<StrategyType, int>> Strategy::GetPattern()
{
    return _pattern;
}

/**
 * @brief Set the new pattern
 *
 * @param new_pattern Set _pattern
 */
void Strategy::SetPattern(std::vector<std::pair<StrategyType, int>> new_pattern)
{
    _pattern = new_pattern;
}

/**
 * @brief Add a element to the pattern
 *
 * @param element New element to add
 */
void Strategy::AddElementToPattern(std::pair<StrategyType, int> element)
{
    _pattern.push_back(element);
}

/**
 * @brief Add a element to the pattern
 *
 * @param action Action of the new element to add
 * @param value Value of the new element to add
 */
void Strategy::AddElementToPattern(StrategyType action, int value)
{
    _pattern.push_back({action,value});
}

/**
 * @brief Create and return a random pattern
 *
 * @param number_of_element Number of element in the pattern
 * @param max_element_value Maximum value for the element
 * @param is_infinitly_looping Set _is_infinitly looping
 * @return std::vector<std::pair<StrategyType, int>> Pattern return
 */
std::vector<std::pair<StrategyType, int>> Strategy::CreateRandomPattern(int number_of_element, int max_element_value, bool is_infinitly_looping)
{
    std::vector<std::pair<StrategyType, int>> random_pattern;

    for (int i = 0; i < number_of_element; i++) {
        random_pattern.push_back({StrategyType(rand() % NUMBER_STRATEGY_POSSIBILITY), rand() % (max_element_value + 1)});
    }
    if (is_infinitly_looping)
        random_pattern.push_back({StrategyType::LOOP, -1});
    return random_pattern;
}

/**
 * @brief Remove a element from the pattern
 *
 * @param index Index of the element to remove
 */
void Strategy::RemoveElementToPattern(size_t index)
{
    if (index < _pattern.size())
        _pattern.erase(_pattern.begin() + index);
}
