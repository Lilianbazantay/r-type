
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
}
