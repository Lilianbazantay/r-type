
#include "DrawSpriteSystem.hpp"
#include "../Component/Position.hpp"
#include "../Component/Sprite.hpp"
#include "../IComponent.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>


/**
 * @brief constructor of DrawSprite
 */
DrawSpriteSystem::DrawSpriteSystem() {
    this->requiedComponents.push_back(ComponentType::SPRITE);
    this->requiedComponents.push_back(ComponentType::POSITION);
}

/**
 * @brief Drawn the given sprite on the window at the given position
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
 */
void DrawSpriteSystem::executeEntity(IMediatorEntity &entity, relevant_data_t &data) {
    std::cout << "Drawing" << std::endl;
    Position *playerPos = dynamic_cast<Position*>(entity.FindComponent(ComponentType::POSITION));
    Sprite *playerSprite = dynamic_cast<Sprite*>(entity.FindComponent(ComponentType::SPRITE));
    if (playerSprite == nullptr) {
        std::cout << "NULL" << std::endl;
        if (entity.FindComponent(ComponentType::SPRITE) != nullptr)
            std::cout << "Find succeeded" << std::endl;
        return;
    }
    sf::Sprite spr = playerSprite->GetSprite();
    auto pos = playerPos->GetPosition();
    std::cout << pos.first << ", " << pos.second << std::endl;
    spr.setPosition(pos.first, pos.second);
    data.window.draw(spr);
}
