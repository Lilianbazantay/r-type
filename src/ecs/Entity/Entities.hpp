#pragma once

#include <memory>
#include <string>
#include "../../server/EntityFactory.hpp"
#include "./IMediatorEntity.hpp"

class Background : public IMediatorEntity {
public:
    explicit Background(EntityFactory& factory);
    Background() = default;
    ~Background() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Player : public IMediatorEntity {
public:
    explicit Player(EntityFactory& factory);
    Player() = default;
    ~Player() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Enemy : public IMediatorEntity {
public:
    explicit Enemy(EntityFactory& factory);
    Enemy() = default;
    ~Enemy() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class PlayerBullet : public IMediatorEntity {
public:
    explicit PlayerBullet(EntityFactory& factory);
    PlayerBullet() = default;
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

class Wall1 : public IMediatorEntity {
public:
    explicit Wall1(EntityFactory& factory);
    ~Wall1() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Wall2 : public IMediatorEntity {
public:
    explicit Wall2(EntityFactory& factory);
    ~Wall2() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Wall3 : public IMediatorEntity {
public:
    explicit Wall3(EntityFactory& factory);
    ~Wall3() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};

class Wall4 : public IMediatorEntity {
public:
    explicit Wall4(EntityFactory& factory);
    ~Wall4() override = default;

    void run() override;
    std::unique_ptr<IMediatorEntity> Clone(EntityFactory& factory) const;
};