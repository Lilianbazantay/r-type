#pragma once

#include <unordered_set>
#include <string>
#include <cstdint>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../client.hpp"

class InputManager
{
private:
    std::unordered_set<int> keysPressed;
    std::string textBuffer;
    Client* _client = nullptr;

    uint8_t mapKeyToNetwork(sf::Keyboard::Key key);

public:
    InputManager();
    ~InputManager() = default;

    void setClient(Client* client);

    void processEvent(const sf::Event& event);

    bool isKeyPressed(sf::Keyboard::Key key) const;

    std::string consumeTextBuffer();
};
