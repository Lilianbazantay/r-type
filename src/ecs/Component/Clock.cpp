#include "Clock.hpp"

/**
 * @brief Return the elapse time since the creation of the clock
 *
 * @return sf::Time
 */
sf::Time Clock::GetElapseTime()
{
    return _clock.getElapsedTime();
}

/**
 * @brief Restart the clock
 */
void Clock::RestartClock()
{
    _clock.restart();
}
