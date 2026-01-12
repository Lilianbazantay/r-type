#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <deque>
#include <utility>
#include <algorithm>
#include <cstdint>

#include "../ecs/System/ISystem.hpp"
#include "../ecs/relevant_data.hpp"

#include "./client.hpp"
#include "./NetworkBuffer.hpp"
#include "./graphical/InputManager.hpp"

class ClientGame {
    private:
        InputManager _inputManager;
        relevant_data_t data;
        std::vector<std::unique_ptr<ISystem>> systemList;
        sf::Time Prevtime;
        sf::Clock clock;
        bool Stopping = false;
        bool Paused = false;
        std::mutex pause_mutex;
        int _localPlayerId = -1;

        NetworkBuffer *_netBuffer = nullptr;
        Client client;

        struct Snapshot {
            uint32_t tick;
            std::pair<float, float> pos;
        };

        struct NetInterpBuffer {
            std::deque<Snapshot> snaps;
        };

        std::unordered_map<uint32_t, NetInterpBuffer> _netBuffers;
        uint32_t _latestServerTick = 0;

        static constexpr uint32_t INTERP_DELAY_TICKS = 6;
        static constexpr uint32_t MAX_EXTRAP_TICKS   = 6;
        static constexpr size_t   MAX_SNAPSHOTS      = 32;

        static uint32_t makeKey(uint8_t type, uint16_t id) {
            return (uint32_t(type) << 16) | uint32_t(id);
        }

        void pushSnapshot(uint8_t type, uint16_t id, uint32_t tick, float x, float y);
        void applyNetworkInterpolation();

    public:
        ClientGame(std::string ip, int port, NetworkBuffer *netBuffer);
        ~ClientGame() = default;

        void createEntity(int, int, std::pair<float, float>);
        void moveEntity(int, int, std::pair<float, float>);
        void deleteEntity(int, int);

        bool isPaused();
        void setPaused(bool);
        bool isStop();
        void setStop(bool);

        void Update();
        void Loop();
        void processNetworkPackets();

        void predictLocalMove(float, float);
};
