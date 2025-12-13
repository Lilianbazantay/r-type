#include "InputManager.hpp"
#include <iostream>

InputManager::InputManager() : textBuffer("")
{
    _keyBindings[Action::Up]    = sf::Keyboard::Z;
    _keyBindings[Action::Down]  = sf::Keyboard::S;
    _keyBindings[Action::Left]  = sf::Keyboard::Q;
    _keyBindings[Action::Right] = sf::Keyboard::D;
    _keyBindings[Action::Fire]  = sf::Keyboard::Space;
}

void InputManager::setClient(Client* client)
{
    _client = client;
}

void InputManager::bindKey(Action action, sf::Keyboard::Key key)
{
    _keyBindings[action] = key;
}

sf::Keyboard::Key InputManager::getKey(Action action) const
{
    auto it = _keyBindings.find(action);
    if (it != _keyBindings.end())
        return it->second;
    return sf::Keyboard::Unknown;
}

bool InputManager::isActionPressed(Action action) const
{
    auto it = _keyBindings.find(action);
    if (it == _keyBindings.end()) return false;
    return keysPressed.find(it->second) != keysPressed.end();
}

void InputManager::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == getKey(Action::Up)) {
            std::cout << "KEY PRESSER UP\n";
            _client->sendInput(true, static_cast<uint8_t>(Action::Up));
        }
        if (event.key.code == getKey(Action::Down))
        {
            _client->sendInput(true, static_cast<uint8_t>(Action::Down));
        }
        if (event.key.code == getKey(Action::Left))
        {
            _client->sendInput(true, static_cast<uint8_t>(Action::Left));
        }
        if (event.key.code == getKey(Action::Right))
        {
            _client->sendInput(true, static_cast<uint8_t>(Action::Right));
        }
        if (event.key.code == getKey(Action::Fire))
        {
            _client->sendInput(true, static_cast<uint8_t>(Action::Fire));
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == getKey(Action::Up)) {
            std::cout << "KEY RELEASE UP\n";
            _client->sendInput(false, static_cast<uint8_t>(Action::Up));
        }
        if (event.key.code == getKey(Action::Down))
        {
            _client->sendInput(false, static_cast<uint8_t>(Action::Down));
        }
        if (event.key.code == getKey(Action::Left))
        {
            _client->sendInput(false, static_cast<uint8_t>(Action::Left));
        }
        if (event.key.code == getKey(Action::Right))
        {
            _client->sendInput(false, static_cast<uint8_t>(Action::Right));
        }
        if (event.key.code == getKey(Action::Fire))
        {
            _client->sendInput(false, static_cast<uint8_t>(Action::Fire));
        }
    }
//    switch (event.type) {
//        case sf::Event::KeyPressed: {
//            if (keysPressed.find(event.key.code) == keysPressed.end()) {
//                keysPressed.insert(event.key.code);
//
//                Action act = keyToAction(event.key.code);
//                std::cout << "Key PRESSED\n";
//                if (act != Action::Unknown) {
//                    _client->sendInput(true, static_cast<uint8_t>(act));
//                }
//                if (event.key.code == sf::Keyboard::X && _client) {
//                    _client->sendStartGame();
//                    std::cout << "START\n";
//                }
//            }
//            break;
//        }
//
//        case sf::Event::KeyReleased: {
//            keysPressed.erase(event.key.code);
//
//            Action act = keyToAction(event.key.code);
//            std::cout << "Key RELEASE\n";
//            if (act != Action::Unknown) {
//                _client->sendInput(false, static_cast<uint8_t>(act));
//            }
//            break;
//        }
//
//        case sf::Event::TextEntered:
//            if (event.text.unicode >= 32u) {
//                textBuffer.push_back(static_cast<char>(event.text.unicode));
//            }
//            break;
//
//        default:
//            break;
//    }
}

std::string InputManager::consumeTextBuffer()
{
    std::string out = textBuffer;
    textBuffer.clear();
    return out;
}

Action InputManager::keyToAction(sf::Keyboard::Key key) const
{
    for (const auto& [act, k] : _keyBindings) {
        if (k == key)
            return act;
    }
    return Action::Unknown;
}
