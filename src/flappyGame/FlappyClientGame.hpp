#pragma once

#include "../client/client.hpp"
#include "../ecs/relevant_data.hpp"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>


class FlappyClientGame {
public:
    FlappyClientGame(std::string ip, int port, NetworkBuffer* buf);
    void Loop();

private:
    void processPackets();

    relevant_data_t data;
    Client client;
    NetworkBuffer* netBuf;
    sf::Texture _texBird;
    sf::Sprite  _sprBird;
    sf::Texture _texBackground;
    sf::Sprite  _sprBackground;
};
