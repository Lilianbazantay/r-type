#pragma once

#include <memory>
#include <string>
#include "../../server/EntityFactory.hpp"
#include "./IMediatorEntity.hpp"

class Background : public IMediatorEntity {
public:
    explicit Background(EntityFactory& factory);
    ~Background() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Player : public IMediatorEntity {
public:
    explicit Player(EntityFactory& factory);
    ~Player() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Enemy : public IMediatorEntity {
public:
    explicit Enemy(EntityFactory& factory);
    ~Enemy() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class PlayerBullet : public IMediatorEntity {
public:
    explicit PlayerBullet(EntityFactory& factory);
    ~PlayerBullet() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class EnemyBullet : public IMediatorEntity {
public:
    explicit EnemyBullet(EntityFactory& factory);
    ~EnemyBullet() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};
