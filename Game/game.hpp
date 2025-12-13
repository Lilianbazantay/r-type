
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <vector>

#include "../src/ecs/Entity/IMediatorEntity.hpp"
#include "../src/ecs/System/ISystem.hpp"
#include "../src/ecs/relevant_data.hpp"

class Background: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a player, ...
    public:
        Background();
        ~Background() = default;
        void run() override;
        IMediatorEntity *Clone() override;
};

class Client {
    private:
        sf::Keyboard::Key Up;
        sf::Keyboard::Key Left;
        sf::Keyboard::Key Down;
        sf::Keyboard::Key Right;
    public:
        Client() {
            Up = sf::Keyboard::Z;
            Left = sf::Keyboard::Q;
            Down = sf::Keyboard::S;
            Right = sf::Keyboard::D;
        };
        ~Client();
        sf::Keyboard::Key getUp() {return Up;};
        sf::Keyboard::Key getLeft() {return Left;};
        sf::Keyboard::Key getDown() {return Down;};
        sf::Keyboard::Key getRight() {return Right;};
        void getUp(sf::Keyboard::Key _key) {Up = _key;};
        void getLeft(sf::Keyboard::Key _key) {Left = _key;};
        void getDown(sf::Keyboard::Key _key) {Down = _key;};
        void getRight(sf::Keyboard::Key _key) {Right = _key;};
};

class Game {
    private:
        relevant_data_t data;
        sf::Time Prevtime;
        std::vector<std::unique_ptr<ISystem>> systemList;
        sf::Clock clock;
        Client _client;
        bool Running = false;
        void createEntities();
        void getInputs(sf::Event);
    public:
        Game();
        ~Game() = default;
        void Network();
        void Update();
        void Loop();
};

