#pragma once

#include <unordered_set>
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "../client.hpp"

class ClientGame;

/**
 * @brief action type enum
 *
 */
enum Action {
    Fire,
    Up,
    Down,
    Left,
    Right,
    Unknown
};

/**
 * @brief class that handles input
 *
 */
class InputManager
{
private:
    std::vector<sf::Keyboard::Key> _keyBindings;
    std::unordered_set<sf::Keyboard::Key> keysPressed;

    std::string textBuffer;
    Client* _client = nullptr;
    ClientGame* _clientGame = nullptr;

    Action keyToAction(sf::Keyboard::Key key) const;

public:
    InputManager();
    ~InputManager() = default;

    void setClient(Client* client);
    void setClientGame(ClientGame* game);

    void bindKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getKey(Action action) const;

    bool isActionPressed(Action action) const;

    void processEvent(const sf::Event& event);

    std::string consumeTextBuffer();
};
