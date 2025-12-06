
#include "DrawAnimatedSystem.hpp"

DrawAnimatedSystem::DrawAnimatedSystem() {
    this->requiedComponents.push_back(ComponentType::ANIMATED_SPRITE);
    this->requiedComponents.push_back(ComponentType::CLOCK);
    this->requiedComponents.push_back(ComponentType::POSITION);
}

void DrawAnimatedSystem::executeEntity(IMediatorEntity &entity) {

}
