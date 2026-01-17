#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <vector>

#include "../ecs/System/ISystem.hpp"
#include "../ecs/relevant_data.hpp"

#include "ecs/Entity/IMediatorEntity.hpp"
#include "server.hpp"
#include "server/NetworkServerBuffer.hpp"
#include "server/WaveManager.hpp"
#include "server/encoder.hpp"
#include "WaveManager.hpp"

class ServerGame {
    private:
        relevant_data_t data;
        std::vector<std::unique_ptr<ISystem>> systemList;
        sf::Time Prevtime;
        sf::Clock clock;
        bool Running = false;

        NetworkServerBuffer *networkReceiveBuffer;
        NetworkClientBuffer *networkSendBuffer;
        NetworkContinuousBuffer *continuousBuffer;
        Server networkServer;
        PacketEncoder encoder;
        WaveManager waveManager;

    public:
        ServerGame(int, NetworkServerBuffer *, NetworkClientBuffer *, NetworkContinuousBuffer *);
        ~ServerGame() = default;
        bool createEntity(int, int);
        void changePlayerDirection(int, std::pair<int, int>);
        void playerShoot(int);
        void parseNetworkPackets();
        void requestGameData();
        void Update();
        void Loop();
        int bulletID = 0;
};
