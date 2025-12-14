#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <vector>

#include "../ecs/System/ISystem.hpp"
#include "../ecs/relevant_data.hpp"

#include "server.hpp"

class ServerGame {
    private:
        relevant_data_t data;
        std::vector<std::unique_ptr<ISystem>> systemList;
        sf::Time Prevtime;
        sf::Clock clock;
        bool Running = false;

        NetworkServerBuffer *networkReceiveBuffer;
        NetworkBuffer *networkSendBuffer;
        Server networkServer;

    public:
        ServerGame(int, NetworkServerBuffer *, NetworkBuffer *);
        ~ServerGame() = default;
        bool createEntity(int, int);
        void changePlayerDirection(int, std::pair<int, int>);
        void parseNetworkPackets();
        void Update();
        void Loop();
};
