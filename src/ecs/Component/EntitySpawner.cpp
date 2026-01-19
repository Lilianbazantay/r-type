#include "EntitySpawner.hpp"

/**
 * @brief Construct a new Entity Spawner object
 *
 * @param cooldown_lenth Cooldown length (seconds)
 * @param entity Entity prototype to spawn
 * @param factory EntityFactory used for cloning
 * @param is_activated Enable or disable the spawner
 */
EntitySpawner::EntitySpawner(
    double cooldown_lenth,
    std::unique_ptr<IMediatorEntity> entity,
    EntityFactory& factory,
    bool is_activated
)
    : _entity(std::move(entity)),
      _factory(factory),
      length(cooldown_lenth),
      _is_activated(is_activated)
{
    _type = ComponentType::ENTITY_SPAWNER;
}

/**
 * @brief Return the cooldown length
 */
double EntitySpawner::GetCooldownLenth()
{
    return length;
}

/**
 * @brief Set the cooldown length
 */
void EntitySpawner::SetCooldownLenth(double new_cooldown)
{
    length = new_cooldown;
}

/**
 * @brief Clone and return a new spawned entity
 */
std::unique_ptr<IMediatorEntity> EntitySpawner::GetEntity()
{
    if (!_entity)
        return nullptr;

    return _entity->Clone(_factory);
}

/**
 * @brief Replace the prototype entity
 */
void EntitySpawner::SetEntity(std::unique_ptr<IMediatorEntity> new_entity)
{
    _entity = std::move(new_entity);
}

/**
 * @brief Check if the spawner is activated
 */
bool EntitySpawner::IsActivated()
{
    return _is_activated;
}

/**
 * @brief Enable the spawner
 */
void EntitySpawner::Enable()
{
    _is_activated = true;
}

/**
 * @brief Disable the spawner
 */
void EntitySpawner::Disable()
{
    _is_activated = false;
}

/**
 * @brief React to the spawn of an entity
 */
void EntitySpawner::Spawn()
{
    can_spawn = false;
}

void EntitySpawner::enableSpawn()
{
    can_spawn = true;
}

/**
 * @brief Return if an entity can spawn
 */
bool EntitySpawner::CanSpawn()
{
    return can_spawn;
}
