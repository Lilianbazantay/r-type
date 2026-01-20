#include "Entities.hpp"
#include "../../server/EntityFactory.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"

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

void BlankEntity::run() {}

std::unique_ptr<IMediatorEntity> BlankEntity::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<BlankEntity>();
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}


Wall1::Wall1(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Wall1", *this);
    this->setType(ENTITY_ENEMY);
}

Wall2::Wall2(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Wall2", *this);
    this->setType(ENTITY_ENEMY);
}

Wall3::Wall3(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Wall3", *this);
    this->setType(ENTITY_ENEMY);
}

Wall4::Wall4(EntityFactory& factory) {
    factory.applyPrototypeToEntity("Wall4", *this);
    this->setType(ENTITY_ENEMY);
}

void Wall1::run() {}

std::unique_ptr<IMediatorEntity> Wall1::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Wall1>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

void Wall2::run() {}

std::unique_ptr<IMediatorEntity> Wall2::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Wall2>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

void Wall3::run() {}

std::unique_ptr<IMediatorEntity> Wall3::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Wall3>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}

void Wall4::run() {}

std::unique_ptr<IMediatorEntity> Wall4::Clone(EntityFactory& factory) const {
    auto clone = std::make_unique<Wall4>(factory);
    clone->setId(id);
    clone->setType(Entitytype);
    return clone;
}
