#include "EntitySpawner.hpp"

/**
 * @brief Construct a new Entity Spawner:: Entity Spawner object
 *
 * @param cooldown_lenth Cooldown lenth of the entity spawner (in second)
 * @param entity Entity that will spawn
 * @param number_of_spawn Number of entity the spawner can spawn (if -1 not limit)
 * @param is_activated Enable or disable the spawner
 */
EntitySpawner::EntitySpawner(double cooldown_lenth, std::unique_ptr<IMediatorEntity> entity, int is_activated)
    : _entity(std::move(entity)), length(cooldown_lenth), _is_activated(is_activated)
{
    _type = ComponentType::ENTITY_SPAWNER;
}

/**
 * @brief Return the cooldown lenth
 *
 * @return double
 */
double EntitySpawner::GetCooldownLenth()
{
    return length;
}

/**
 * @brief Set the cooldown lenth
 *
 * @param new_cooldown Set _cooldown.lenth
 */
void EntitySpawner::SetCooldownLenth(double new_cooldown)
{
    length = new_cooldown;
}

/**
 * @brief Return the Entity the will be spawn
 *
 * @return IMediatorEntity* Entity stocked
 */
std::unique_ptr<IMediatorEntity> EntitySpawner::GetEntity()
{
    return _entity->Clone();
}

/**
 * @brief Replace the entity with a new
 *
 * @param new_entity Set _entity
 */
void EntitySpawner::SetEntity(std::unique_ptr<IMediatorEntity> new_entity)
{
    _entity = std::move(new_entity);
}

/**
 * @brief Return if the Spawner is activated
 *
 * @return true The spawner is enable
 * @return false The spawner is disable
 */
bool EntitySpawner::IsActivated()
{
    return _is_activated;
}

/**
 * @brief Enable the Spawner
 */
void EntitySpawner::Enable()
{
    _is_activated = true;
}

/**
 * @brief Disable the Spawner
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
 * @brief Return if an entity can spawn or not
 *
 * @return true An entity can spawn
 * @return false An entity can not spawn
 */
bool EntitySpawner::CanSpawn()
{
    return can_spawn;
}
