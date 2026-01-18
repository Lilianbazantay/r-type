
#include "StrategySystem.hpp"
#include "../IComponent.hpp"
#include "ecs/Component/EntitySpawner.hpp"
#include "ecs/Component/Direction.hpp"
#include "ecs/Component/Hp.hpp"
#include "ecs/Component/Strategy.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>


/**
 * @brief constructor of Strategy
 */
StrategySystem::StrategySystem() {
    this->requiedComponents.push_back(ComponentType::STRATEGY);
    this->requiedComponents.push_back(ComponentType::DIRECTION);
    this->requiedComponents.push_back(ComponentType::HP);

    this->optionnalComponents.push_back(ComponentType::ENTITY_SPAWNER);
}

/**
 * @brief modifies the direction of the entity, can optionnally shoot
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
 */
void StrategySystem::executeEntity(IMediatorEntity &entity, relevant_data_t &data) {
    if (!entity.is_Alive())
        return;

    Strategy *playerStrategy = dynamic_cast<Strategy*>(entity.FindComponent(ComponentType::STRATEGY));
    Direction *playerDirection = dynamic_cast<Direction*>(entity.FindComponent(ComponentType::DIRECTION));
    Hp *playerHp = dynamic_cast<Hp*>(entity.FindComponent(ComponentType::HP));
    EntitySpawner *playerEntitySpawner = dynamic_cast<EntitySpawner*>(entity.FindComponent(ComponentType::ENTITY_SPAWNER));

    if (playerHp == nullptr || playerDirection == nullptr || playerStrategy == nullptr) {
        std::cerr << "[WARN][StrategySystem] Entity id=" << entity.getId()
                  << " type=" << entity.getType()
                  << " missing Strategy/DIRECTION/HP component\n";
        return;
    }
    std::pair<float, float> dir = playerStrategy->getDir(playerHp->GetHpPercent());
    if (dir.first == 2 && playerEntitySpawner != nullptr)
        return playerEntitySpawner->enableSpawn();
    playerDirection->SetDirection(dir);
}
