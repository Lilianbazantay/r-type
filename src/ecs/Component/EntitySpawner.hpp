#pragma once

#include "../IComponent.hpp"
#include "../Entity/IMediatorEntity.hpp"
#include "../../server/EntityFactory.hpp"
#include "Cooldown.hpp"

class EntitySpawner : public IComponent
{
private:
    std::unique_ptr<IMediatorEntity> _entity;
    EntityFactory& _factory;

    bool can_spawn = false;
    double length;
    bool _is_activated;

public:
    EntitySpawner(
        double cooldown_lenth,
        std::unique_ptr<IMediatorEntity> entity,
        EntityFactory& factory,
        bool is_activated
    );

    ~EntitySpawner() override = default;

    // GET / SET
    double GetCooldownLenth();
    void SetCooldownLenth(double new_cooldown);

    std::unique_ptr<IMediatorEntity> GetEntity();
    void SetEntity(std::unique_ptr<IMediatorEntity> new_entity);

    bool IsActivated();
    void Enable();
    void Disable();

    // SPAWN
    void enableSpawn();
    void Spawn();
    bool CanSpawn();
};
