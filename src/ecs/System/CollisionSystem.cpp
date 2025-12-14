
#include "CollisionSystem.hpp"

#include "../Component/Hitbox.hpp"
#include "../Component/Position.hpp"
#include "../Component/Hp.hpp"

#include "../Entity/IMediatorEntity.hpp"
#include "../IComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>

/**
 * @brief Basic constructor of collision
 */
CollisionSystem::CollisionSystem() {
    this->requiedComponents.push_back(ComponentType::POSITION);
    this->requiedComponents.push_back(ComponentType::HITBOX);

    this->optionnalComponents.push_back(ComponentType::HP);
}

/**
 *
 * @brief check layers between two entities
 *
 * @returns true if entities shares a layer
 * @returns false if entities do not share a layer
 */
bool CollisionSystem::checkLayers(std::vector<int> masks, std::vector<int> layers) {
    for (size_t i = 0; i < masks.size(); i++)
        for (size_t j = 0; j < layers.size(); j++)
            if (masks[i] == layers[j])
                return true;
    return false;
}

/**
 * @brief check collisions between two entities
 *
 * @returns true if entities collide
 * @returns false if entities do not collide
 */
bool CollisionSystem::checkCollison(std::pair<float, float> playerPos, std::pair<float, float> playerSize,
    std::pair<float, float> entityPos, std::pair<float, float> entitySize) {

    if ((playerPos.first < entityPos.first || playerPos.first > entityPos.first + entitySize.first)
        && (playerPos.first + playerSize.first < entityPos.first || playerPos.first + playerSize.first > entityPos.first + entitySize.first))
        return false;
    if ((playerPos.second < entityPos.second || playerPos.second > entityPos.second + entitySize.second)
        && (playerPos.second + playerSize.second < entityPos.second || playerPos.second + playerSize.second > entityPos.second + entitySize.second))
        return false;
    return true;
}

/**
 * @brief performs a collision and layer check upon every entity. if entites collide and have the same layer, damages colliders based on mask; and if they have HP
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
 */
void CollisionSystem::executeEntity(IMediatorEntity &entity, relevant_data_t &data) {
    Hitbox *playerHitbox = dynamic_cast<Hitbox*>(entity.FindComponent(ComponentType::HITBOX));
    Position *playerPosition = dynamic_cast<Position*>(entity.FindComponent(ComponentType::POSITION));
    Hp *playerHp = dynamic_cast<Hp*>(entity.FindComponent(ComponentType::HP));

    playerHitbox->GetMask();
    playerHitbox->GetLayers();

    Hitbox *entityHitbox;
    Position *entityPosition;
    Hp *entityHp;
    std::vector<std::unique_ptr<IMediatorEntity>>& entityList = data.entityList;
    for (size_t i = 0; i < entityList.size(); i++) {
        entityHitbox = dynamic_cast<Hitbox*>(entityList[i]->FindComponent(ComponentType::HITBOX));
        entityPosition = dynamic_cast<Position*>(entityList[i]->FindComponent(ComponentType::POSITION));
        if (entityHitbox == nullptr || entityPosition == nullptr)
            continue;
        if (!checkCollison(playerPosition->GetPosition(), playerHitbox->GetHitboxSize(),
            entityPosition->GetPosition(), entityHitbox->GetHitboxSize())
            || !checkCollison(entityPosition->GetPosition(), entityHitbox->GetHitboxSize(),
            playerPosition->GetPosition(), playerHitbox->GetHitboxSize()))
            continue;
        if (playerHp != nullptr && checkLayers(playerHitbox->GetMask(), entityHitbox->GetLayers())) {
            playerHp->SubHp(entityHitbox->GetDamage());
            if (playerHp->GetHp() <= 0)
                entity.Alive(false);
        }
        entityHp = dynamic_cast<Hp*>(entityList[i]->FindComponent(ComponentType::HP));
        if (entityHp != nullptr && checkLayers(entityHitbox->GetMask(), playerHitbox->GetLayers())) {
            entityHp->SubHp(playerHitbox->GetDamage());
            if (entityHp->GetHp() <= 0)
                entityList[i]->Alive(false);
        }
    }
}
