#include "Window.hpp"

/**
 * @brief Construct a new Window Manager:: Window Manager object
 *
 * @param title name of the window
 * @param width width of the window
 * @param height height of the window
 */
WindowManager::WindowManager(const std::string& title, unsigned int width, unsigned int height)
{
    sf::VideoMode vm(width, height);
    _window.create(vm, title, sf::Style::Default);
    _window.setVerticalSyncEnabled(true);
}

/**
 * @brief tell if the window is open
 *
 * @return true window is open
 * @return false window is closed
 */
bool WindowManager::isOpen() const
{
    return _window.isOpen();
}

/**
 * @brief get the window
 *
 * @return sf::RenderWindow& window
 */
sf::RenderWindow& WindowManager::get()
{
    return _window;
}

/**
 * @brief return size of the window
 *
 * @return sf::Vector2u size of the window
 */
sf::Vector2u WindowManager::getSize()
{
    return _window.getSize();
}

/**
 * @brief resize the window
 * 
 * @param width new width
 * @param height new height
 */
void WindowManager::handleResize(unsigned int width, unsigned int height)
{
    sf::FloatRect visibleArea(0, 0, static_cast<float>(width), static_cast<float>(height));
    _window.setView(sf::View(visibleArea));
}
