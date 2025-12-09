#include "Cooldown.hpp"
#include <chrono>

/**
 * @brief Construct a new Cooldown
 */
Cooldown::Cooldown(double lenth)
    : _lenth(lenth)
{
    _type = ComponentType::COOLDOWN;
}

/**
 * @brief Return current time in seconds (microsecond precision)
 *
 * @param double Time return
 */
double Cooldown::GetTimeSeconds()
{
    using clock = std::chrono::steady_clock;

    auto now = clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()
    ).count();

    return static_cast<double>(us) / 1'000'000.0;
}

/**
 * @brief Get the lenth of the cooldown (in seconds)
 *
 * @return double Return the lenth of the cooldown
 */
double Cooldown::GetLenth()
{
    return _lenth;
}

/**
 * @brief Set the lenth of the cooldown (in seconds)
 *
 * @param new_lenth New lenth of cooldown
 */
void Cooldown::SetLenth(double new_lenth)
{
    _lenth = new_lenth;
}

/**
 * @brief Start the cooldown
 */
void Cooldown::LaunchCooldown()
{
    _timestamp_activation = GetTimeSeconds();
}

/**
 * @brief Check if the cooldown is over
 *
 * @return true cooldown finished
 * @return false cooldown still running
 */
bool Cooldown::CheckCooldown()
{
    if (_timestamp_activation < 0)
        return true;

    double elapsed = GetTimeSeconds() - _timestamp_activation;

    if (elapsed >= _lenth) {
        _remaning = 0;
        return true;
    }

    _remaning = _lenth - elapsed;
    return false;
}
