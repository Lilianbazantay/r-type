#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>


#include "Window.hpp"
#include "InputManager.hpp"


class Game
{
private:
    WindowManager window;
    InputManager input;

    sf::CircleShape player;

    bool useText = false;
    sf::Font font;
    sf::Text info;
    std::string typedText;

    void update(float dt);
    std::string pressedKeysToString() const;

public:
    Game();
    void run();
};
