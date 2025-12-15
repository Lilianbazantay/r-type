
#include "ShootSystem.hpp"
#include "../Entity/IMediatorEntity.hpp"
#include "../IComponent.hpp"

#include "../Component/Position.hpp"
#include "../Component/Sprite.hpp"
#include "../Component/Velocity.hpp"
#include "../Component/Clock.hpp"
#include "../Component/EntitySpawner.hpp"
#include <cerrno>
#include <iostream>
#include <memory>

/**
 * @brief constructor of DrawAnimatedSprite
 */
ShootSystem::ShootSystem() {
    this->requiedComponents.push_back(ComponentType::ENTITY_SPAWNER);
    this->requiedComponents.push_back(ComponentType::POSITION);

    this->optionnalComponents.push_back(ComponentType::CLOCK);
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
    Sprite *playerSize = dynamic_cast<Sprite*>(entity.FindComponent(ComponentType::SPRITE));
    Velocity *playerVelocity = dynamic_cast<Velocity*>(entity.FindComponent(ComponentType::VELOCITY));
    EntitySpawner *newEntityData = dynamic_cast<EntitySpawner*>(entity.FindComponent(ComponentType::ENTITY_SPAWNER));
    Clock *playerClock = dynamic_cast<Clock*>(entity.FindComponent(ComponentType::CLOCK));
    if (newEntityData == nullptr || playerPos == nullptr || playerVelocity == nullptr || !newEntityData->CanSpawn())
        return;
    if (playerClock != nullptr && newEntityData->GetCooldownLenth() > playerClock->getTimeMilliseconds())
        return;
    newEntityData->Spawn();
    if (playerClock != nullptr)
        playerClock->RestartClock();
    std::unique_ptr<IMediatorEntity> newEntity = newEntityData->GetEntity();
    if (newEntity == nullptr) {
        return;
    }
    Position *bulletPos = dynamic_cast<Position*>(newEntity->FindComponent(ComponentType::POSITION));
    if (bulletPos != nullptr) {
        auto pos = playerPos->GetPosition();
        auto size = playerSize->GetSize();

        float offsetX = size.first;
        float offsetY = size.second / 2.0f - 3.5f;

        bulletPos->SetPosition({
            pos.first + offsetX,
            pos.second + offsetY
        });
    }
    newEntity->setId(data.bullet_count);
    data.bullet_count++;
    data.entityList.push_back(std::move(newEntity));
}
