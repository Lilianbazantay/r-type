#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <mutex>
#include <vector>

#include "./IMediatorEntity.hpp"

class Background: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a player, ...

        int id;
        std::mutex _mutex;
        public:
        Background();
        ~Background() = default;
        void run() override;
        IMediatorEntity *Clone() override;
};

class Player: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a player, ...

        int id;
        std::mutex _mutex;
    public:
        Player();
        ~Player() = default;
        void run() override;
        IMediatorEntity *Clone() override;
};

class Enemy: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a Enemy, ...

        int id;
        std::mutex _mutex;
    public:
        Enemy();
        ~Enemy() = default;
        void run() override;
        IMediatorEntity *Clone() override;
};


class PlayerBullet: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a player, ...

        int id;
        std::mutex _mutex;
    public:
        PlayerBullet();
        ~PlayerBullet() = default;
        void run() override;
        IMediatorEntity *Clone() override;
};
