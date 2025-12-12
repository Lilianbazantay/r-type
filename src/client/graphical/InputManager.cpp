#include "InputManager.hpp"


InputManager::InputManager() : textBuffer("") {}

void InputManager::processEvent(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            keysPressed.insert(event.key.code);
            break;
        case sf::Event::KeyReleased:
            keysPressed.erase(event.key.code);
            break;
        case sf::Event::TextEntered:
            if (event.text.unicode >= 32u) {
                textBuffer.push_back(static_cast<char>(event.text.unicode));
            }
            break;
        default:
            break;
    }
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
    return keysPressed.find(key) != keysPressed.end();
}

std::string InputManager::consumeTextBuffer()
{
    std::string out = textBuffer;
    textBuffer.clear();
    return out;
}
