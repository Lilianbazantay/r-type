
#include "Entities.hpp"
#include "../Component/Position.hpp"
#include "../Component/Direction.hpp"
#include "../Component/Sprite.hpp"
#include "../Component/Velocity.hpp"
#include "../Component/Hp.hpp"
#include "../Component/Hitbox.hpp"
#include "ecs/Component/Clock.hpp"
#include "ecs/Component/EntitySpawner.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include <memory>


Background::Background() {
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/stellar_background.jpg", 1920, 1080));
    this->setType(ENTITY_BACKGROUND);
}

void Background::run() {}

std::unique_ptr<IMediatorEntity> Background::Clone() {
    auto clone = std::make_unique<Background>();

        // Copy basic data
        clone->id = id;
        clone->Entitytype = Entitytype;
        clone->entity_created = true;
        clone->entity_changed = entity_changed;
        clone->entity_live = entity_live;

        // Deep copy components
        for (const auto &c : _actuatorComponents)
            clone->_actuatorComponents.push_back(c);

        for (const auto &c : _undergoerComponents)
            clone->_undergoerComponents.push_back(c);

        clone->_actuatorTypeList = _actuatorTypeList;
        clone->_undergoerTypeList = _undergoerTypeList;
        return clone;
}


Enemy::Enemy() {
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Enemy.png", 64, 64));
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->AddActuatorComponent(std::make_unique<Velocity>(1));
    this->AddActuatorComponent(std::make_unique<Position>(1800, 500));
    this->AddActuatorComponent(std::make_unique<Hitbox>(64, 64, 1, std::vector<int>{2}, std::vector<int>{1}));
    this->setType(ENTITY_ENEMY);
}

void Enemy::run() {}


std::unique_ptr<IMediatorEntity> Enemy::Clone() {
    auto clone = std::make_unique<Enemy>();

        // Copy basic data
        clone->id = id;
        clone->Entitytype = Entitytype;
        clone->entity_created = true;
        clone->entity_changed = entity_changed;
        clone->entity_live = entity_live;

        // Deep copy components
        for (const auto &c : _actuatorComponents)
            clone->_actuatorComponents.push_back(c);

        for (const auto &c : _undergoerComponents)
            clone->_undergoerComponents.push_back(c);

        clone->_actuatorTypeList = _actuatorTypeList;
        clone->_undergoerTypeList = _undergoerTypeList;
        return clone;
}


PlayerBullet::PlayerBullet() {
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Bullet.png", 8, 8));
    this->AddActuatorComponent(std::make_unique<Direction>(1, 0));
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->AddActuatorComponent(std::make_unique<Velocity>(500));
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Hitbox>(8, 8, 1, std::vector<int>{}, std::vector<int>{2}));
    this->setType(ENTITY_BULLET);
}

void PlayerBullet::run() {}


std::unique_ptr<IMediatorEntity> PlayerBullet::Clone() {
    auto clone = std::make_unique<PlayerBullet>();

        // Copy basic data
        clone->id = id;
        clone->Entitytype = Entitytype;
        clone->entity_created = true;
        clone->entity_changed = entity_changed;
        clone->entity_live = entity_live;

        // Deep copy components
        for (const auto &c : _actuatorComponents)
            clone->_actuatorComponents.push_back(c);

        for (const auto &c : _undergoerComponents)
            clone->_undergoerComponents.push_back(c);

        clone->_actuatorTypeList = _actuatorTypeList;
        clone->_undergoerTypeList = _undergoerTypeList;
        return clone;
}


Player::Player() {
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Player.png", 64, 64));
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<EntitySpawner>(2000, std::make_unique<PlayerBullet>(), true));
    this->AddActuatorComponent(std::make_unique<Hp>(3));
    this->AddActuatorComponent(std::make_unique<Velocity>(100));
    this->AddActuatorComponent(std::make_unique<Position>(100, 200));
    this->AddActuatorComponent(std::make_unique<Hitbox>(64, 64, 1, std::vector<int>{1}, std::vector<int>{2}));

    this->AddActuatorComponent(std::make_unique<Clock>());
    this->setType(ENTITY_PLAYER);
}

void Player::run() {}


std::unique_ptr<IMediatorEntity> Player::Clone() {
    auto clone = std::make_unique<Player>();

        // Copy basic data
        clone->id = id;
        clone->Entitytype = Entitytype;
        clone->entity_created = true;
        clone->entity_changed = entity_changed;
        clone->entity_live = entity_live;

        // Deep copy components
        for (const auto &c : _actuatorComponents)
            clone->_actuatorComponents.push_back(c);

        for (const auto &c : _undergoerComponents)
            clone->_undergoerComponents.push_back(c);

        clone->_actuatorTypeList = _actuatorTypeList;
        clone->_undergoerTypeList = _undergoerTypeList;
        return clone;
}
