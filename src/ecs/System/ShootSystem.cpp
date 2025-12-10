
#include "ShootSystem.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"

#include "../Component/Position.hpp"
#include "../Component/Velocity.hpp"
#include "../Component/EntitySpawner.hpp"
#include <cerrno>

/**
 * @brief constructor of DrawAnimatedSprite
 */
ShootSystem::ShootSystem() {
    this->requiedComponents.push_back(ComponentType::ENTITY_SPAWNER);
    this->requiedComponents.push_back(ComponentType::CLOCK);
    this->requiedComponents.push_back(ComponentType::POSITION);

    this->optionnalComponents.push_back(ComponentType::VELOCITY);
}

/**
 * @brief Drawn the given animated sprite on the window at the given position
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
 */
void ShootSystem::executeEntity(IMediatorEntity &entity, relevant_data_t &data) {
    Position *playerPos = dynamic_cast<Position*>(entity.FindComponent(ComponentType::POSITION));
    Velocity *playerVelocity = dynamic_cast<Velocity*>(entity.FindComponent(ComponentType::VELOCITY));
    EntitySpawner *newEntityData = dynamic_cast<EntitySpawner*>(entity.FindComponent(ComponentType::ENTITY_SPAWNER));
    if (newEntityData == nullptr || playerPos == nullptr || playerVelocity == nullptr || !newEntityData->CanSpawn())
        return;
    newEntityData->Spawn();
    IMediatorEntity *newEntity = newEntityData->GetEntity();
    data.entityList.push_back(newEntity);
}
