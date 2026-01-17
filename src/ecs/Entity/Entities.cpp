
#include "Entities.hpp"
#include "../Component/Position.hpp"
#include "../Component/Direction.hpp"
#include "../Component/Sprite.hpp"
#include "../Component/Velocity.hpp"
#include "../Component/Hp.hpp"
#include "../Component/Hitbox.hpp"
#include "ecs/Component/AnimatedSprite.hpp"
#include "ecs/Component/Clock.hpp"
#include "ecs/Component/EntitySpawner.hpp"
#include "ecs/Component/Strategy.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"
#include <SFML/System/Vector2.hpp>
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
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Enemy.png", 96, 96));
    this->AddActuatorComponent(std::make_unique<Hitbox>(128, 128, 1, std::vector<int>{2}, std::vector<int>{1}));
    this->AddActuatorComponent(std::make_unique<Position>(1500, 500));
    this->AddActuatorComponent(std::make_unique<Strategy>(10, 4, true));

    this->AddActuatorComponent(std::make_unique<Velocity>(80));
    this->AddActuatorComponent(std::make_unique<EntitySpawner>(2000, std::make_unique<EnemyBullet>(), true));
    this->AddActuatorComponent(std::make_unique<Clock>());
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->setType(ENTITY_ENEMY);
}

Enemy::Enemy(Sprite _spr, Hitbox _hitbox, Position _pos, Strategy _strat, int velocity, int hp) {
    this->AddActuatorComponent(std::make_unique<Sprite>(_spr));
    this->AddActuatorComponent(std::make_unique<Hitbox>(_hitbox));
    this->AddActuatorComponent(std::make_unique<Position>(_pos));
    this->AddActuatorComponent(std::make_unique<Strategy>(_strat));

    this->AddActuatorComponent(std::make_unique<Velocity>(velocity));
    this->AddActuatorComponent(std::make_unique<EntitySpawner>(2000, std::make_unique<EnemyBullet>(), true));
    this->AddActuatorComponent(std::make_unique<Clock>());
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<Hp>(hp));
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
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Bullet.png", 16, 16));
    this->AddActuatorComponent(std::make_unique<Direction>(1, 0));
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->AddActuatorComponent(std::make_unique<Velocity>(500));
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Hitbox>(16, 16, 1, std::vector<int>{1}, std::vector<int>{2}));
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



EnemyBullet::EnemyBullet() {
    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Bullet.png", 128, 128));
    this->AddActuatorComponent(std::make_unique<Direction>(-1, 0));
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->AddActuatorComponent(std::make_unique<Velocity>(500));
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Hitbox>(16, 16, 1, std::vector<int>{2}, std::vector<int>{1}));
    this->setType(ENTITY_BULLET);
}

void EnemyBullet::run() {}


std::unique_ptr<IMediatorEntity> EnemyBullet::Clone() {
    auto clone = std::make_unique<EnemyBullet>();

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
    this->AddActuatorComponent(std::make_unique<AnimatedSprite>("./assets/trantorian.png", std::pair<float, float>{1, 1}, std::pair<float, float>{76, 92}));
    AnimatedSprite *Asprite = dynamic_cast<AnimatedSprite*>(FindComponent(ComponentType::ANIMATED_SPRITE));
    Asprite->addAnimation({76, 92}, {0, 0}, {76, 0}, ANIMATION_TYPE::IDLE, true, 3);
    Asprite->changeAnimation(IDLE, true);
    //    this->AddActuatorComponent(std::make_unique<AnimatedSprite>(Asprite));
//    this->AddActuatorComponent(std::make_unique<Sprite>("./assets/Player.png", 64, 64));
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<EntitySpawner>(2000, std::make_unique<PlayerBullet>(), true));
    this->AddActuatorComponent(std::make_unique<Hp>(3));
    this->AddActuatorComponent(std::make_unique<Velocity>(400));
    this->AddActuatorComponent(std::make_unique<Position>(100, 200));
    this->AddActuatorComponent(std::make_unique<Hitbox>(128, 128, 1, std::vector<int>{1}, std::vector<int>{2}));

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
