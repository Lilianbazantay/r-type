#include "InputManager.hpp"
#include <cstddef>
#include <iostream>
#include "../ClientGame.hpp"

/**
 * @brief Construct a new Input Manager:: Input Manager object
 *
 */
InputManager::InputManager() : textBuffer("")
{
    _keyBindings.resize(Action::Unknown);
    _keyBindings.at(Action::Up) = sf::Keyboard::Z;
    _keyBindings.at(Action::Down) = sf::Keyboard::S;
    _keyBindings.at(Action::Left)  = sf::Keyboard::Q;
    _keyBindings.at(Action::Right) = sf::Keyboard::D;
    _keyBindings.at(Action::Fire)  = sf::Keyboard::Space;
}

/**
 * @brief set the client of the input manager
 *
 * @param client new client
 */
void InputManager::setClient(Client* client)
{
    _client = client;
}

void InputManager::setClientGame(ClientGame* game)
{
    _clientGame = game;
}

/**
 * @brief bind a key to an action
 *
 * @param action action to be bound with
 * @param key key to bind
 */
void InputManager::bindKey(Action action, sf::Keyboard::Key key)
{
    _keyBindings[action] = key;
}

/**
 * @brief get the key that was bound to an action
 *
 * @param action action needed
 * @return sf::Keyboard::Key key bound to the action
 */
sf::Keyboard::Key InputManager::getKey(Action action) const
{
    if (action != _keyBindings.size())
        return _keyBindings.at(action);
    return sf::Keyboard::Unknown;
}

/**
 * @brief check if an action happens because a key is pressed or not
 *
 * @param action action involved
 * @return true it is pressed
 * @return false it is released
 */
bool InputManager::isActionPressed(Action action) const
{
    if (action == _keyBindings.size()) return false;
    return keysPressed.find(_keyBindings.at(action)) != keysPressed.end();
}

/**
 * @brief process an event and send an input based off it
 *
 * @param event event to be processed
 */
void InputManager::processEvent(const sf::Event& event)
{
    if (!_client)
        return;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == getKey(Action::Up)) {
            _client->sendInput(true, static_cast<uint8_t>(Action::Up));
            if (_clientGame) _clientGame->predictLocalMove(0.f, -1.f);
        }
        if (event.key.code == getKey(Action::Down)) {
            _client->sendInput(true, static_cast<uint8_t>(Action::Down));
            if (_clientGame) _clientGame->predictLocalMove(0.f, 1.f);
        }
        if (event.key.code == getKey(Action::Left)) {
            _client->sendInput(true, static_cast<uint8_t>(Action::Left));
            if (_clientGame) _clientGame->predictLocalMove(-1.f, 0.f);
        }
        if (event.key.code == getKey(Action::Right)) {
            _client->sendInput(true, static_cast<uint8_t>(Action::Right));
            if (_clientGame) _clientGame->predictLocalMove(1.f, 0.f);
        }
        if (event.key.code == getKey(Action::Fire))
            _client->sendInput(true, static_cast<uint8_t>(Action::Fire));
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == getKey(Action::Up))
            _client->sendInput(false, static_cast<uint8_t>(Action::Up));
        if (event.key.code == getKey(Action::Down))
            _client->sendInput(false, static_cast<uint8_t>(Action::Down));
        if (event.key.code == getKey(Action::Left))
            _client->sendInput(false, static_cast<uint8_t>(Action::Left));
        if (event.key.code == getKey(Action::Right))
            _client->sendInput(false, static_cast<uint8_t>(Action::Right));
        if (event.key.code == getKey(Action::Fire))
            _client->sendInput(false, static_cast<uint8_t>(Action::Fire));
    }
}

/**
 * @brief clear the text buffer
 *
 * @return std::string cleared out buffer
 */
std::string InputManager::consumeTextBuffer()
{
    std::string out = textBuffer;
    textBuffer.clear();
    return out;
}

/**
 * @brief egt an action from a key
 *
 * @param key key
 * @return Action action
 */
Action InputManager::keyToAction(sf::Keyboard::Key key) const
{
    for (size_t i = 0; i < _keyBindings.size(); i++)
        if (_keyBindings.at(i) == key)
            return (Action)i;
    return Action::Unknown;
}
