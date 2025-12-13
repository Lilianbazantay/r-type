#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

class WindowManager
{
private:
    sf::RenderWindow _window;

public:
    WindowManager(const std::string& title, unsigned int width = 800, unsigned int height = 600);

    bool isOpen() const;
    void close() { _window.close(); }

    sf::RenderWindow& get();

    sf::Vector2u getSize();

    void handleResize(unsigned int width, unsigned int height);
};
