#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <mutex>
#include <vector>

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

        NetworkBuffer *_netBuffer = nullptr;
        Client client;

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
};
