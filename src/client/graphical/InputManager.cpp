#include "InputManager.hpp"

InputManager::InputManager() : textBuffer("") {}

void InputManager::setClient(Client* client) {
    _client = client;
}

void InputManager::processEvent(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed: {
            if (keysPressed.find(event.key.code) == keysPressed.end()) {
                keysPressed.insert(event.key.code);

                if (_client) {
                    uint8_t code = mapKeyToNetwork(event.key.code);
                    if (code != 255)
                        _client->sendInput(true, code);
                }
            }
            break;
        }

        case sf::Event::KeyReleased: {
            keysPressed.erase(event.key.code);

            if (_client) {
                uint8_t code = mapKeyToNetwork(event.key.code);
                if (code != 255)
                    _client->sendInput(false, code);
            }
            break;
        }

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

uint8_t InputManager::mapKeyToNetwork(sf::Keyboard::Key key)
{
    switch (key) {
        case sf::Keyboard::Space:
            return 0;
        case sf::Keyboard::Up:
            return 1;
        case sf::Keyboard::Down:
            return 2;
        case sf::Keyboard::Left:
            return 3;
        case sf::Keyboard::Right:
            return 4;
        case sf::Keyboard::X:
            _client->sendStartGame();
        default:
            return 255;
    }
}
