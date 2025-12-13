
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

#include "../ecs/Entity/Entities.hpp"
#include "ecs/Component/Strategy.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"

class ClientInputs {
    private:
        sf::Keyboard::Key Up;
        sf::Keyboard::Key Left;
        sf::Keyboard::Key Down;
        sf::Keyboard::Key Right;
    public:
        ClientInputs() {
            Up = sf::Keyboard::Z;
            Left = sf::Keyboard::Q;
            Down = sf::Keyboard::S;
            Right = sf::Keyboard::D;
        };
        ~ClientInputs() = default;
        sf::Keyboard::Key getUp() {return Up;};
        sf::Keyboard::Key getLeft() {return Left;};
        sf::Keyboard::Key getDown() {return Down;};
        sf::Keyboard::Key getRight() {return Right;};
        void getUp(sf::Keyboard::Key _key) {Up = _key;};
        void getLeft(sf::Keyboard::Key _key) {Left = _key;};
        void getDown(sf::Keyboard::Key _key) {Down = _key;};
        void getRight(sf::Keyboard::Key _key) {Right = _key;};
};

class ClientGame {
    private:
        ClientInputs clientInputs;
        relevant_data_t data;
        std::vector<std::unique_ptr<ISystem>> systemList;
        sf::Time Prevtime;
        sf::Clock clock;
        bool Stopping = false;
        bool Paused = false;
        std::mutex pause_mutex;
        void getInputs(sf::Event);
    public:
        ClientGame();
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
};
