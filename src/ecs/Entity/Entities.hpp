#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <mutex>
#include <vector>

#include "./IMediatorEntity.hpp"
#include "ecs/Component/Hitbox.hpp"
#include "ecs/Component/Position.hpp"
#include "ecs/Component/Sprite.hpp"
#include "ecs/Component/Strategy.hpp"

class Background: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;
        std::vector<IComponent> _actuatorComponents;
        std::vector<IMediatorEntity> _attachedEntities;

        std::mutex _mutex;
    public:
        Background();
        ~Background() override = default;
        void run() override;
        std::unique_ptr<IMediatorEntity> Clone() override;
};

class Player: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;
        std::vector<IComponent> _actuatorComponents;
        std::vector<IMediatorEntity> _attachedEntities;

        std::mutex _mutex;
    public:
        Player();
        ~Player() override = default;
        void run() override;
        std::unique_ptr<IMediatorEntity> Clone() override;
};

class Enemy: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;
        std::vector<IComponent> _actuatorComponents;
        std::vector<IMediatorEntity> _attachedEntities;

        std::mutex _mutex;
    public:
        Enemy();
        Enemy(Sprite, Hitbox, Position, Strategy, int, int);
        ~Enemy() override = default;
        void run() override;
        std::unique_ptr<IMediatorEntity> Clone() override;
};


class PlayerBullet: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;
        std::vector<IComponent> _actuatorComponents;
        std::vector<IMediatorEntity> _attachedEntities;

        std::mutex _mutex;
    public:
        PlayerBullet();
        ~PlayerBullet() override = default;
        void run() override;
        std::unique_ptr<IMediatorEntity> Clone() override;
};


class EnemyBullet: public IMediatorEntity {
    private:
        std::vector<IComponent> _undergoerComponents;
        std::vector<IComponent> _actuatorComponents;
        std::vector<IMediatorEntity> _attachedEntities;

        std::mutex _mutex;
    public:
        EnemyBullet();
        ~EnemyBullet() override = default;
        void run() override;
        std::unique_ptr<IMediatorEntity> Clone() override;
};
