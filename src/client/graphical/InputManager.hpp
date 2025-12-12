#pragma once

#include <unordered_set>
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Vector2.hpp>


class InputManager
{
private:
    std::unordered_set<int> keysPressed;
    std::string textBuffer;


public:
    InputManager();
    ~InputManager() = default;

    void processEvent(const sf::Event& event);

    bool isKeyPressed(sf::Keyboard::Key key) const;

    std::string consumeTextBuffer();
};
