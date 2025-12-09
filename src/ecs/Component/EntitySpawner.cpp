#include "EntitySpawner.hpp"

/**
 * @brief Construct a new Entity Spawner:: Entity Spawner object
 *
 * @param cooldown_lenth Cooldown lenth of the entity spawner (in second)
 * @param entity Entity that will spawn
 * @param number_of_spawn Number of entity the spawner can spawn (if -1 not limit)
 * @param is_activated Enable or disable the spawner
 */
EntitySpawner::EntitySpawner(double cooldown_lenth, IMediatorEntity *entity, int number_of_spawn, int is_activated)
    : _cooldown(cooldown_lenth), _entity(entity), _nb_of_spawn(number_of_spawn), _is_activated(is_activated)
{
    _type = ComponentType::ENTITY_SPAWNER;
}

/**
 * @brief Return the component cooldown
 *
 * @return Cooldown is the component
 */
Cooldown EntitySpawner::GetCooldown()
{
    return _cooldown;
}

/**
 * @brief Set the cooldown component with a new
 *
 * @param new_cooldown Set _cooldown
 */
void EntitySpawner::SetCooldown(Cooldown new_cooldown)
{
    _cooldown = new_cooldown;
}

/**
 * @brief Return the cooldown lenth
 *
 * @return double
 */
double EntitySpawner::GetCooldownLenth()
{
    return _cooldown.GetLenth();
}

/**
 * @brief Set the cooldown lenth
 *
 * @param new_cooldown Set _cooldown.lenth
 */
void EntitySpawner::SetCooldownLenth(double new_cooldown)
{
    _cooldown.SetLenth(new_cooldown);
}

/**
 * @brief Return the Entity the will be spawn
 *
 * @return IMediatorEntity* Entity stocked
 */
IMediatorEntity *EntitySpawner::GetEntity()
{
    return _entity;
}

/**
 * @brief Replace the entity with a new
 *
 * @param new_entity Set _entity
 */
void EntitySpawner::SetEntity(IMediatorEntity *new_entity)
{
    _entity = new_entity;
}

/**
 * @brief Return the number of entity that the spawner can spawn
 *
 * @return int Number of spawn
 */
int EntitySpawner::GetNumberOfSpawn()
{
    return _nb_of_spawn;
}

/**
 * @brief Replace the number of spawn
 *
 * @param new_number_of_spawn Set _nb_of_spawn
 */
void EntitySpawner::SetNumberOfSpawn(int new_number_of_spawn)
{
    _nb_of_spawn = new_number_of_spawn;
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
    _nb_of_spawn -= 1;
    _cooldown.LaunchCooldown();
}

/**
 * @brief Return if an entity can spawn or not
 *
 * @return true An entity can spawn
 * @return false An entity can not spawn
 */
bool EntitySpawner::CanSpawn()
{
    if (_cooldown.CheckCooldown() == true && _nb_of_spawn == 0)
        return true;
    return false;
}
