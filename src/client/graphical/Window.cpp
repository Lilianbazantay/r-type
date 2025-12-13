#include "Window.hpp"


WindowManager::WindowManager(const std::string& title, unsigned int width, unsigned int height)
{
    sf::VideoMode vm(width, height);
    _window.create(vm, title, sf::Style::Default);
    _window.setVerticalSyncEnabled(true);
}

bool WindowManager::isOpen() const
{
    return _window.isOpen();
}

sf::RenderWindow& WindowManager::get()
{
    return _window;
}

sf::Vector2u WindowManager::getSize()
{
    return _window.getSize();
}

void WindowManager::handleResize(unsigned int width, unsigned int height)
{
    sf::FloatRect visibleArea(0, 0, static_cast<float>(width), static_cast<float>(height));
    _window.setView(sf::View(visibleArea));
}
