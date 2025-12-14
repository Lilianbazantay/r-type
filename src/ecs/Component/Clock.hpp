#pragma once

#include "../IComponent.hpp"
#include <SFML/System/Clock.hpp>

class Clock : public IComponent
{
private:
    sf::Clock _clock;

public:
    Clock() = default;
    ~Clock() override = default;

    sf::Time GetElapseTime();
    void RestartClock();
};
