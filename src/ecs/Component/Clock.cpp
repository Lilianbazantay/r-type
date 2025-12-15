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

int Clock::getTimeMilliseconds()
{
    return _clock.getElapsedTime().asMilliseconds();
}


/**
 * @brief Restart the clock
 */
void Clock::RestartClock()
{
    _clock.restart();
}
