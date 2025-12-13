
#include "Entities.hpp"
#include "../Component/Position.hpp"
#include "../Component/Direction.hpp"
#include "../Component/Sprite.hpp"
#include "../Component/Velocity.hpp"
#include "../Component/Hp.hpp"
#include "../Component/Hitbox.hpp"


Background::Background() {
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Sprite>("../../../assets/stellar_background.jpg", 1920, 1080));
}

void Background::run() {}

IMediatorEntity *Background::Clone() {
    return nullptr;
}

Player::Player() {
    this->AddActuatorComponent(std::make_unique<Sprite>("../../../assets/dialogus.png", 64, 64));
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<Hp>(3));
    this->AddActuatorComponent(std::make_unique<Velocity>(1));
    this->AddActuatorComponent(std::make_unique<Position>(100, 200));
    this->AddActuatorComponent(std::make_unique<Hitbox>(64, 64, 1, std::vector<int>{1}, std::vector<int>{2}));
}

void Player::run() {}


IMediatorEntity *Player::Clone() {
    return nullptr;
}


Enemy::Enemy() {
    this->AddActuatorComponent(std::make_unique<Sprite>("../../../assets/dialogus.png", 64, 64));
    this->AddActuatorComponent(std::make_unique<Direction>());
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->AddActuatorComponent(std::make_unique<Velocity>(1));
    this->AddActuatorComponent(std::make_unique<Position>(1800, 500));
    this->AddActuatorComponent(std::make_unique<Hitbox>(64, 64, 1, std::vector<int>{2}, std::vector<int>{1}));
}

void Enemy::run() {}


IMediatorEntity *Enemy::Clone() {
    return nullptr;
}


PlayerBullet::PlayerBullet() {
    this->AddActuatorComponent(std::make_unique<Sprite>("../../../assets/dialogus.png", 8, 8));
    this->AddActuatorComponent(std::make_unique<Direction>(0, 2));
    this->AddActuatorComponent(std::make_unique<Hp>(1));
    this->AddActuatorComponent(std::make_unique<Velocity>(350));
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Hitbox>(8, 8, 1, std::vector<int>{}, std::vector<int>{2}));
}

void PlayerBullet::run() {}


IMediatorEntity *PlayerBullet::Clone() {
    return nullptr;
}
