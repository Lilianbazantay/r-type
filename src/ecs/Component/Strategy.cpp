#include "Strategy.hpp"
#include "../IComponent.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>

/**
 * @brief Construct a new Strategy:: Strategy object
 *
 * @param strategy Set the strategy pattern
 */
Strategy::Strategy(std::vector<std::pair<std::pair<float, float>, float>> pattern): _pattern(pattern)
{
    _patternList.push_back({_pattern, 0});
    arrayPos = 0;
    _type = ComponentType::STRATEGY;
}

Strategy::Strategy(int number_of_element, int max_time_value, bool is_infinitly_looping) {
    CreateRandomPattern(number_of_element, max_time_value, 0, is_infinitly_looping);
    arrayPos = 0;
    _type = ComponentType::STRATEGY;
}

void Strategy::switchPattern(int healthPercent) {
    int posPercent = 0;
    size_t pos = 0;
    for (size_t i = 0; i < _patternList.size(); i++) {
        if (_patternList[i].second <= healthPercent && _patternList[i].second > posPercent)
            pos = i;
    }
    _pattern = _patternList[pos].first;
    _patternHp = _patternList[pos].second;
    arrayPos = 0;
    clock.restart();
}

/**
 * @brief Return the pattern
 *
 * @return std::vector<std::pair<std::pair<float, float>, float>> Pattern type returned
 */
std::vector<std::pair<std::pair<float, float>, float>> Strategy::GetPatternFromHp(int healthPercent)
{
    if (_patternList.empty())
        return {};
    int posPercent = 0;
    size_t pos = 0;
    for (size_t i = 0; i < _patternList.size(); i++) {
        if (_patternList[i].second <= healthPercent && _patternList[i].second > posPercent)
            pos = i;
    }
    return _patternList[pos].first;
}

/**
 * @brief Set the new pattern
 *
 * @param new_pattern Set _pattern
 */
void Strategy::SetPattern(std::vector<std::pair<std::pair<float, float>, float>> new_pattern, int healthPercent)
{
    _patternList.push_back({new_pattern, healthPercent});
}

/**
 * @brief Add a element to the pattern
 *
 * @param element New element to add
 */
void Strategy::AddElementToPattern(std::pair<std::pair<float, float>, float> element, int healthPercent)
{
    for (size_t i = 0; i < _patternList.size(); i++)
        if (_patternList[i].second == healthPercent) {
            _patternList[i].first.push_back(element);
            return;
        }
    _patternList.push_back({{element}, healthPercent});
}

/**
 * @brief Add a element to the pattern
 *
 * @param action Action of the new element to add
 * @param value Value of the new element to add
 */
void Strategy::AddElementToPattern(std::pair<float, float> action, int value, int healthPercent)
{
    for (size_t i = 0; i < _patternList.size(); i++) {
        if (_patternHp == healthPercent) {
            _pattern.push_back({action, value});
            return;
        }
    }
    _patternList.push_back({{{action, value}}, healthPercent});
}

/**
 * @brief Create and return a random pattern
 *
 * @param number_of_element Number of element in the pattern
 * @param max_time_value Maximum time for the element (in seconds)
 * @param is_infinitly_looping Set _is_infinitly looping
 */
void Strategy::CreateRandomPattern(int number_of_element, int max_time_value,  int healthPercent, bool is_infinitly_looping)
{
    std::vector<std::pair<std::pair<float, float>, float>> random_pattern = {};

    for (int i = 0; i < number_of_element; i++) {
        int actRand = rand();
        if (actRand % 5 == 0) {
            std::cout << "shooty" << std::endl;
            random_pattern.push_back({{2, 2}, 0});
        } else {
            std::pair<float, float> newPath = std::pair<float, float>((actRand % 3) - 1, (rand() % 3) - 1);
            random_pattern.push_back({newPath, rand() % (max_time_value + 1)});
        }
    }
    if (is_infinitly_looping)
        random_pattern.push_back({{-2, -2}, -1});
    _patternList.push_back({random_pattern, healthPercent});
}

/**
 * @brief getThe actual Direction of the entity
 *
 * @return direction or {0, 0} if pattern is empty
 */
std::pair<float, float> Strategy::getDir(int healthPercent) {
    if (healthPercent < _patternHp)
        switchPattern(healthPercent);
    if (_pattern.size() == 0)
        return {0, 0};
    if (clock.getElapsedTime().asSeconds() > _pattern[arrayPos].second) {
        clock.restart();
        arrayPos++;
        if (_pattern[arrayPos].first.first == -2)
            arrayPos = 0;
    }
    //std::cout << "going in\n";
    if (arrayPos >= _pattern.size())
        return {0, 0};
    return _pattern[arrayPos].first;
}

/**
 * @brief Remove a element from the pattern
 *
 * @param index Index of the element to remove
 */
void Strategy::RemoveElementToPattern(size_t index, int healthPercent)
{
    for (size_t i = 0; i < _patternList.size(); i++)
        if (_patternList[i].second == healthPercent && index < _pattern.size()) {
            _pattern.erase(_pattern.begin() + index);
            return;
        }
}
