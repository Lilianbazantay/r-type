#pragma once

#include "../IComponent.hpp"
#include "../Entity/IMediatorEntity.hpp"
#include "Cooldown.hpp"

class EntitySpawner : public IComponent
{
private:
    Cooldown _cooldown;
    IMediatorEntity *_entity;
    int _nb_of_spawn;
    bool _is_activated;

public:
    EntitySpawner(double cooldown_lenth, IMediatorEntity *entity, int number_of_spawn, int is_activated);
    ~EntitySpawner() override = default;

    // GET / SET
    Cooldown GetCooldown();
    void SetCooldown(Cooldown new_cooldown);
    double GetCooldownLenth();
    void SetCooldownLenth(double new_cooldown);

    IMediatorEntity *GetEntity();
    void SetEntity(IMediatorEntity *new_entity);

    int GetNumberOfSpawn();
    void SetNumberOfSpawn(int new_number_of_spawn);

    bool IsActivated();
    void Enable();
    void Disable();

    // SPAWN
    void Spawn();
    bool CanSpawn();
};
