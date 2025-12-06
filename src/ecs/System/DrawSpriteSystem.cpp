
#include "DrawSpriteSystem.hpp"

DrawSpriteSystem::DrawSpriteSystem() {
    this->requiedComponents.push_back(ComponentType::SPRITE);
    this->requiedComponents.push_back(ComponentType::POSITION);
}

void DrawSpriteSystem::executeEntity(IMediatorEntity &entity) {

}
