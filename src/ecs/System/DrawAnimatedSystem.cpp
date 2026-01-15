#include "../Component/Position.hpp"
#include "../Component/Sprite.hpp"
#include "../IComponent.hpp"

#include "DrawAnimatedSystem.hpp"

/**
 * @brief constructor of DrawAnimatedSprite
 */
DrawAnimatedSystem::DrawAnimatedSystem() {
    this->requiedComponents.push_back(ComponentType::ANIMATED_SPRITE);
    this->requiedComponents.push_back(ComponentType::CLOCK);
    this->requiedComponents.push_back(ComponentType::POSITION);
}

/**
 * @brief Drawn the given animated sprite on the window at the given position
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
 */
void DrawAnimatedSystem::executeEntity(IMediatorEntity &entity, relevant_data_t &data) {
    //std::cout << "Drawing" << std::endl;
    if (!entity.is_Alive())
        return;
    Position *playerPos = dynamic_cast<Position*>(entity.FindComponent(ComponentType::POSITION));
    Sprite *playerSprite = dynamic_cast<Sprite*>(entity.FindComponent(ComponentType::SPRITE));
    if (playerSprite == nullptr)
        return;
    sf::Sprite& spr = playerSprite->GetSprite();
    auto pos = playerPos->GetPosition();
    //std::cout << pos.first << ", " << pos.second << std::endl;
    spr.setPosition(pos.first, pos.second);
    data.window.draw(spr);
}
