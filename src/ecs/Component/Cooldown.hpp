#pragma once

#include "../IComponent.hpp"

/**
 * @brief Cooldown class
 * @note Time is stored in seconds (with millisecond precision)
 */
class Cooldown : public IComponent
{
private:
    double _lenth;
    double _remaning = -1;
    double _timestamp_activation = -1;

    double GetTimeSeconds();

public:
    Cooldown(double lenth);
    ~Cooldown() override = default;

    double GetLenth();
    void SetLenth(double new_lenth);

    void LaunchCooldown();
    bool CheckCooldown();
};
