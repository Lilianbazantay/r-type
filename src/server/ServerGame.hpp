#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <string>
#include <vector>
#include <atomic>

#include "../ecs/System/ISystem.hpp"
#include "../ecs/relevant_data.hpp"

#include "ecs/Entity/IMediatorEntity.hpp"
#include "server.hpp"
#include "server/NetworkServerBuffer.hpp"
#include "server/WaveManager.hpp"
#include "server/encoder.hpp"

#include "EntityFactory.hpp"

class ServerGame {
    private:
        relevant_data_t data;
        std::vector<std::unique_ptr<ISystem>> systemList;
        sf::Time Prevtime;
        sf::Clock clock;
        std::atomic_bool Running{false};
        uint32_t _serverTick = 0;

    EntityFactory factory;
    bool g_sigint = false;

    NetworkServerBuffer* networkReceiveBuffer;
    NetworkClientBuffer* networkSendBuffer;
    NetworkContinuousBuffer* continuousBuffer;
    Server networkServer;
    PacketEncoder encoder;
    WaveManager waveManager;

    void updateCreation(std::unique_ptr<IMediatorEntity> &entity);
    void updateDeath(std::unique_ptr<IMediatorEntity> &entity, size_t pos);
    void updateModifications(std::unique_ptr<IMediatorEntity> &entity);

public:
    ServerGame(int port, NetworkServerBuffer* newRBuffer, NetworkClientBuffer* newSBuffer, NetworkContinuousBuffer* newCBuffer);
    ~ServerGame() = default;

    bool createEntity(int entity_type, int personnal_id, std::string subType);
    void changePlayerDirection(int personnal_id, std::pair<int, int> newValues);
    void playerShoot(int player_id);
    void parseNetworkPackets();
    void requestGameData();
    void Update();
    void Loop();

    int bulletID = 0;
};
