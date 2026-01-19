#include "../Component/Position.hpp"
#include "../Component/Sprite.hpp"
#include "../IComponent.hpp"
#include "ecs/Component/AnimatedSprite.hpp"
#include <iostream>

#include "DrawAnimatedSystem.hpp"

/**
 * @brief constructor of DrawAnimatedSprite
 */
DrawAnimatedSystem::DrawAnimatedSystem() {
    this->requiedComponents.push_back(ComponentType::ANIMATED_SPRITE);
    this->requiedComponents.push_back(ComponentType::POSITION);
}

/**
 * @brief Drawn the given animated sprite on the window at the given position
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
 */
void DrawAnimatedSystem::executeEntity(IMediatorEntity &entity, relevant_data_t &data) {
    if (!entity.is_Alive())
        return;
    Position *playerPos = dynamic_cast<Position*>(entity.FindComponent(ComponentType::POSITION));
    AnimatedSprite *playerSprite = dynamic_cast<AnimatedSprite*>(entity.FindComponent(ComponentType::ANIMATED_SPRITE));
    if (playerSprite == nullptr || playerPos == nullptr)
        return;
    sf::Sprite& spr = playerSprite->getSprite();
    auto pos = playerPos->GetPosition();
    spr.setPosition(pos.first, pos.second);
    data.window.draw(spr);
}
