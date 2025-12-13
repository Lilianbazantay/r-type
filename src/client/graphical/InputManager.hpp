#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdint>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../client.hpp"

enum class Action : uint8_t {
    Up,
    Down,
    Left,
    Right,
    Fire,
    Unknown
};

class InputManager
{
private:
    std::unordered_map<Action, sf::Keyboard::Key> _keyBindings;
    std::unordered_set<sf::Keyboard::Key> keysPressed;

    std::string textBuffer;
    Client* _client = nullptr;

    Action keyToAction(sf::Keyboard::Key key) const;

public:
    InputManager();
    ~InputManager() = default;

    void setClient(Client* client);

    void bindKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getKey(Action action) const;

    bool isActionPressed(Action action) const;

    void processEvent(const sf::Event& event);

    std::string consumeTextBuffer();
};
