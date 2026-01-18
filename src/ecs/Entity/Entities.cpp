#include "Entities.hpp"
#include "../../server/EntityFactory.hpp"

Background::Background(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Background", *this);
    this->setType(ENTITY_BACKGROUND);
}

void Background::run() {}

std::unique_ptr<IMediatorEntity> Background::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Background>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

Player::Player(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Player", *this);
    this->setType(ENTITY_PLAYER);
}

void Player::run() {}

std::unique_ptr<IMediatorEntity> Player::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Player>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

Enemy::Enemy(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Enemy", *this);
    this->setType(ENTITY_ENEMY);
}

void Enemy::run() {}

std::unique_ptr<IMediatorEntity> Enemy::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Enemy>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

PlayerBullet::PlayerBullet(EntityFactory& factory) {
    factory.applyPrototypeToEntity("PlayerBullet", *this);
    this->setType(ENTITY_BULLET);
}

void PlayerBullet::run() {}

std::unique_ptr<IMediatorEntity> PlayerBullet::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<PlayerBullet>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

EnemyBullet::EnemyBullet(EntityFactory& factory) {
    factory.applyPrototypeToEntity("EnemyBullet", *this);
    this->setType(ENTITY_BULLET);
}

void EnemyBullet::run() {}

std::unique_ptr<IMediatorEntity> EnemyBullet::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<EnemyBullet>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}
